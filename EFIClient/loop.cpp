#include "loop.h"

Loop Loop::loop;

Loop::Loop()
{
    this->pid = 0;
    this->client = 0;
    this->entityOnCrosshair = false;
    for (int i = 0; i < 32; ++i)
        ppPos[i] = new playerPosition();
}

Loop::~Loop()
{
    for (int i = 0; i < 32; ++i)
        delete ppPos[i];
}

void Loop::updateLoop()
{
    int count = 0;
    while (1)
    {
        if (count % 100 == 0)
        {
            for (int i = 0; i < 32; ++i)
                ppPos[i] = new playerPosition();
        }

        uintptr_t localPlayer = driver::read<uintptr_t>(pid, client + offsets::dwLocalPlayerPawn);
        int localTeam;
        if (config::misc.only_enemies)
            localTeam = driver::read<int>(pid, localPlayer + offsets::m_iTeamNum);
        
        if (config::triggerbot.enable)
        {
            int entityId = driver::read<int>(pid, localPlayer + offsets::m_iIDEntIndex);
            if (entityId > 0)
            {
                uintptr_t entList = driver::read<uintptr_t>(pid, client + offsets::dwEntityList);
                uintptr_t entEntry = driver::read<uintptr_t>(pid, entList + 0x8 * (entityId >> 9) + 0x10);
                uintptr_t entity = driver::read<uintptr_t>(pid, entEntry + 120 * (entityId & 0x1FF));

                if (config::misc.only_enemies)
                {
                    int entityTeam = driver::read<int>(pid, entity + offsets::m_iTeamNum);

                    if (entityTeam != localTeam)
                    {
                        int entityHp = driver::read<int>(pid, entity + offsets::m_iHealth);
                        entityOnCrosshair = (entityHp > 0);
                    }
                    else {
                        entityOnCrosshair = false;
                    }
                }
                else if (!config::misc.only_enemies)
                {
                    int entityHp = driver::read<int>(pid, entity + offsets::m_iHealth);
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

        uintptr_t entity_list = driver::read<uintptr_t>(pid, client + offsets::dwEntityList);

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        vector3 Localorigin = driver::read<vector3>(pid, localPlayer + offsets::m_vecOrigin);
        view_matrix_t view_matrix = driver::read<view_matrix_t>(pid, client + offsets::dwViewMatrix);

        for (int i = 0; i < 32; ++i)
        {
            uintptr_t listEntry = driver::read<uintptr_t>(pid, entity_list + (8 * (i & 0x7FFF) >> 9) + 16);
            if (!listEntry) continue;
            uintptr_t player = driver::read<uintptr_t>(pid, listEntry + 120 * (i & 0x1FF));
            if (!player) continue;
            uintptr_t playerPawn = driver::read<uintptr_t>(pid, player + offsets::m_hPlayerPawn);
            uintptr_t listEntry2 = driver::read<uintptr_t>(pid, entity_list + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
            if (!listEntry2) continue;
            uintptr_t pCSPlayerPawn = driver::read<uintptr_t>(pid, listEntry2 + 120 * (playerPawn & 0x1FF));
            if (!pCSPlayerPawn) continue;
            if (pCSPlayerPawn == localPlayer) continue;

            if (config::misc.only_enemies)
            {
                int team = driver::read<int>(pid, pCSPlayerPawn + offsets::m_iTeamNum);
                if (team == localTeam) continue;
            }

            int health = driver::read<int>(pid, pCSPlayerPawn + offsets::m_iHealth);
            if (health <= 0 || health > 100) continue;

            uintptr_t gamescene = driver::read<uintptr_t>(pid, pCSPlayerPawn + 0x318);

            uintptr_t bonearray = driver::read<uintptr_t>(pid, gamescene + 0x160 + 0x80);

            vector3 head = driver::read<vector3>(pid, bonearray + 6 * 32);

            vector3 screenHead = head.wts(view_matrix);

            if (screenHead.x <= 0 || screenHead.y <= 0) continue;

            if (screenHead.x < screenWidth || screenHead.y < screenHeight)
            {
                ppPos[i]->screenPosition[0] = screenHead.x;
                ppPos[i]->screenPosition[1] = screenHead.y;
            }
        }
        count++;
    }
};

bool Loop::isPlayerOnCrosshair()
{
    return entityOnCrosshair;
};

playerPosition** Loop::getPlayerPositions()
{
    return ppPos;
};