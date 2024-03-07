#include "includes.h"
#include "offsets.h"
#include "functions.h"
#include <stdint.h>
#include "arduino.h"
#include "config.h"
#include "mainLoop.h"
#include "features.h"

extern arduino duino;

extern std::string arduino_name;

extern int movement_type;
extern bool only_enemies;
extern int head_position;

extern bool triggerbot;
extern bool aimbot;

int main()
{
    if (!Driver::initialize() || !CheckDriverStatus()) {
        UNICODE_STRING VariableName = RTL_CONSTANT_STRING(VARIABLE_NAME);
        NtSetSystemEnvironmentValueEx(
            &VariableName,
            &DummyGuid,
            0,
            0,
            ATTRIBUTES);

        printf("[Z3BRA] No EFI Driver found\n");
        system("pause");
        exit(1);
        return 1;
    }

    printf("[Z3BRA] EFI Driver found\n");

    ConfigRead();

    printf("[Z3BRA] Config file found\n");

    if (movement_type == 1) {
        if (!duino.initialize(arduino_name.c_str())) {
            system("pause"); 
            exit(1);
        }
    }

    auto hwnd = FindWindowA(NULL, "Counter-Strike 2");
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == 0 || pid == 1 || !pid) {
        printf("[Z3BRA] CS2 not found\n");
        system("pause");
        exit(1);
    }
    printf("[Z3BRA] CS2 found\n");

    uintptr_t client = getModuleAddress(pid, "client.dll");
    if (!client || client == 0 || client == 1) {
        printf("[Z3BRA] client.dll address not found\n");
        system("pause");
        exit(1);
    }
    printf("[Z3BRA] client.dll base module address found\n");

    printf("[Z3BRA] Starting loop thread...\n");

    Loop loopInstance(pid, client, triggerbot, only_enemies, head_position);
    std::thread updateThread(&Loop::updateLoop, &loopInstance);
    updateThread.detach();

    if (movement_type == 1) {
        if (triggerbot && aimbot) {
            std::thread triggerbotThread(Arduino::Triggerbot::start, std::ref(loopInstance));
            std::thread aimbotThread(Arduino::Aimbot::start, std::ref(loopInstance));
            printf("[Z3BRA] Aimbot & Triggerbot threads started\n");
            aimbotThread.join();

        }
        else if (aimbot) {
            std::thread aimbotThread(Arduino::Aimbot::start, std::ref(loopInstance));
            printf("[Z3BRA] Aimbot thread started\n");
            aimbotThread.join();

        }
        else if (triggerbot) {
            std::thread triggerbotThread(Arduino::Triggerbot::start, std::ref(loopInstance));
            printf("[Z3BRA] Triggerbot thread started\n");
            triggerbotThread.join();
        }
        
    } else if (movement_type == 2) {
        if (triggerbot && aimbot) {
            std::thread triggerbotThread(Mouse::Triggerbot::start, std::ref(loopInstance));
            std::thread aimbotThread(Mouse::Aimbot::start, std::ref(loopInstance));
            printf("[Z3BRA] Aimbot & Triggerbot threads started\n");
            aimbotThread.join();

        } else if (aimbot) {
            std::thread aimbotThread(Mouse::Aimbot::start, std::ref(loopInstance));
            printf("[Z3BRA] Aimbot thread started\n");
            aimbotThread.join();

        } else if (triggerbot) {
            std::thread triggerbotThread(Mouse::Triggerbot::start, std::ref(loopInstance));
            printf("[Z3BRA] Triggerbot thread started\n");
            triggerbotThread.join();
        }
    }
    
    printf("[Z3BRA] All cheat features are turned off or problem with config file.\n");
    system("pause");
}
