#pragma once
#include "includes.h"
#include "struct.h"
#include "offsets.h"
#include "functions.h"
#include "config.h"

class Loop
{
public:
    PlayerPosition* ppPos[32];
    bool entityOnCrosshair;
    DWORD pid;
    uintptr_t client;
    bool triggerbot;
    bool onlyEnemies;
    int headpos;

    Loop(DWORD pid, uintptr_t client, bool triggerbot, bool onlyEnemies, int headpos) noexcept {
        this->pid = pid;
        this->client = client;
        this->triggerbot = triggerbot;
        this->onlyEnemies = onlyEnemies;
        this->entityOnCrosshair = false;
        this->headpos = headpos;

        for (int i = 0; i < 32; ++i) {
            ppPos[i] = new PlayerPosition();
        }
    }

    ~Loop() {
        // Deallocate memory for ppPos
        for (int i = 0; i < 32; ++i) {
            delete ppPos[i];
        }
    }

    // Can be used in future to read memory in one call
    bool ReadHugeMemory(std::uintptr_t moduleBase, std::uintptr_t buffer, size_t size) const noexcept
    {
        if (moduleBase == 0)
            return false;

        return Driver::read_memory(pid, (moduleBase), buffer, size) != 0;
    }

    template <typename T>
    const T ReadModuleBuffer(const void* baseAddress, const std::uintptr_t offset) const noexcept
    {
        T value = { };
        ::memcpy(&value, reinterpret_cast<const void*>(reinterpret_cast<std::uintptr_t>(baseAddress) + offset), sizeof(T));
        return value;
    }

    void updateLoop()
    {
        int count = 0;
        while (1)
        {
            if (count % 100 == 0) {
                for (int i = 0; i < 32; ++i) {
                    ppPos[i] = new PlayerPosition();
                }
            }

            uintptr_t localPlayer = Driver::read<uintptr_t>(pid, client + offsets::dwLocalPlayerPawn);
            int localTeam;
            if (onlyEnemies) {
                localTeam = Driver::read<int>(pid, localPlayer + offsets::m_iTeamNum);
            }
            // Triggerbot logic
            if (triggerbot) {
                int entityId = Driver::read<int>(pid, localPlayer + offsets::m_iIDEntIndex);
                if (entityId > 0) {
                    uintptr_t entList = Driver::read<uintptr_t>(pid, client + offsets::dwEntityList);
                    uintptr_t entEntry = Driver::read<uintptr_t>(pid, entList + 0x8 * (entityId >> 9) + 0x10);
                    uintptr_t entity = Driver::read<uintptr_t>(pid, entEntry + 120 * (entityId & 0x1FF));

                    if (onlyEnemies) {
                        int entityTeam = Driver::read<int>(pid, entity + offsets::m_iTeamNum);

                        if (entityTeam != localTeam) {
                            int entityHp = Driver::read<int>(pid, entity + offsets::m_iHealth);
                            entityOnCrosshair = (entityHp > 0);
                        }
                        else {
                            entityOnCrosshair = false;
                        }
                    }
                    else if (!onlyEnemies) {
                        int entityHp = Driver::read<int>(pid, entity + offsets::m_iHealth);
                        entityOnCrosshair = (entityHp > 0);
                    }
                    else {
                        entityOnCrosshair = false;
                    }
                }
                else {
                    entityOnCrosshair = false;
                }
            }

            uintptr_t entity_list = Driver::read<uintptr_t>(pid, client + offsets::dwEntityList);

            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);
            Vector3 Localorigin = Driver::read<Vector3>(pid, localPlayer + offsets::m_vecOrigin);
            view_matrix_t view_matrix = Driver::read<view_matrix_t>(pid, client + offsets::dwViewMatrix);

            for (int i = 0; i < 32; ++i) {
                uintptr_t listEntry = Driver::read<uintptr_t>(pid, entity_list + (8 * (i & 0x7FFF) >> 9) + 16);
                if (!listEntry) continue;
                uintptr_t player = Driver::read<uintptr_t>(pid, listEntry + 120 * (i & 0x1FF));
                if (!player) continue;
                uintptr_t playerPawn = Driver::read<uintptr_t>(pid, player + offsets::m_hPlayerPawn);
                uintptr_t listEntry2 = Driver::read<uintptr_t>(pid, entity_list + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
                if (!listEntry2) continue;
                uintptr_t pCSPlayerPawn = Driver::read<uintptr_t>(pid, listEntry2 + 120 * (playerPawn & 0x1FF));
                if (!pCSPlayerPawn) continue;
                if (pCSPlayerPawn == localPlayer) continue;

                if (onlyEnemies) {
                    int team = Driver::read<int>(pid, pCSPlayerPawn + offsets::m_iTeamNum);
                    if (team == localTeam) continue;
                }

                int health = Driver::read<int>(pid, pCSPlayerPawn + offsets::m_iHealth);
                if (health <= 0 || health > 100) continue;

                Vector3 origin = Driver::read<Vector3>(pid, pCSPlayerPawn + offsets::m_vOldOrigin);
                Vector3 head = { origin.x, origin.y, origin.z + (float)headpos };

                Vector3 screenHead = head.WTS(view_matrix);

                if (screenHead.x <= 0 || screenHead.y <= 0) continue;

                if (screenHead.x < screenWidth || screenHead.y < screenHeight) {
                    ppPos[i]->screenPosition[0] = screenHead.x;
                    ppPos[i]->screenPosition[1] = screenHead.y;
                }
            }
            count++;
        }
    };

    bool isPlayerOnCrosshair() {
        return entityOnCrosshair;
    };

    PlayerPosition** getPlayerPositions() {
        return ppPos;
    };
};