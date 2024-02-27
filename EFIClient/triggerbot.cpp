#include "includes.h"
#include "arduino.h"
#include "offsets.h"
#include "functions.h"

void TriggerbotArduino(int pid, uintptr_t client, int triggerbot_delay_before_click, int triggerbot_delay_after_click)
{

    printf("[Z3BRA] Try connect to the arduino...\n");

    arduino duino("Arduino Leonardo");
    char buffer[] = "-10";

    while (true) {
        if (GetAsyncKeyState(VK_LSHIFT)) {
            uintptr_t player = Driver::read<uintptr_t>(pid, client + offsets::dwLocalPlayerPawn);
            int entityId = Driver::read<int>(pid, player + offsets::m_iIDEntIndex);

            if (entityId > 0) {
                uintptr_t entList = Driver::read<uintptr_t>(pid, client + offsets::dwEntityList);

                uintptr_t entEntry = Driver::read<uintptr_t>(pid, entList + 0x8 * (entityId >> 9) + 0x10);
                uintptr_t entity = Driver::read<uintptr_t>(pid, entEntry + 120 * (entityId & 0x1FF));

                int entityTeam = Driver::read<int>(pid, entity + offsets::m_iTeamNum);
                int playerTeam = Driver::read<int>(pid, player + offsets::m_iTeamNum);

                if (entityTeam != playerTeam) {
                    int entityHp = Driver::read<int>(pid, entity + offsets::m_iHealth);
                    if (entityHp > 0) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(triggerbot_delay_before_click));
                        duino.send_data(buffer, sizeof(buffer));
                        std::this_thread::sleep_for(std::chrono::milliseconds(triggerbot_delay_after_click));
                    }
                }



            }
        }
    }
}

void TriggerbotMouse(int pid, uintptr_t client, int triggerbot_delay_before_click, int triggerbot_delay_after_click)
{

    while (true) {

        if (GetAsyncKeyState(VK_LSHIFT)) {
            uintptr_t player = Driver::read<uintptr_t>(pid, client + offsets::dwLocalPlayerPawn);
            int entityId = Driver::read<int>(pid, player + offsets::m_iIDEntIndex);

            if (entityId > 0) {
                uintptr_t entList = Driver::read<uintptr_t>(pid, client + offsets::dwEntityList);

                uintptr_t entEntry = Driver::read<uintptr_t>(pid, entList + 0x8 * (entityId >> 9) + 0x10);
                uintptr_t entity = Driver::read<uintptr_t>(pid, entEntry + 120 * (entityId & 0x1FF));

                int entityTeam = Driver::read<int>(pid, entity + offsets::m_iTeamNum);
                int playerTeam = Driver::read<int>(pid, player + offsets::m_iTeamNum);

                if (entityTeam != playerTeam) {
                    int entityHp = Driver::read<int>(pid, entity + offsets::m_iHealth);
                    if (entityHp > 0) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(triggerbot_delay_before_click));
                        LeftClick();
                        std::this_thread::sleep_for(std::chrono::milliseconds(triggerbot_delay_after_click));
                    }
                }



            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }
}