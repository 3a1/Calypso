

#include "includes.h"
#include "offsets.h"
#include "functions.h"
#include <stdint.h>
#include "arduino.h"
#include "triggerbot.h"
#include "config.h"
#include "aimbot.h"

int main()
{
    if (!Driver::initialize() || !CheckDriverStatus()) {
        UNICODE_STRING VariableName = RTL_CONSTANT_STRING(VARIABLE_NAME);
        NtSetSystemEnvironmentValueEx(
            &VariableName,
            &DummyGuid,
            0,
            0,
            ATTRIBUTES);//delete var

        printf("No EFI Driver found\n");
        system("pause");
        exit(1);
        return 1;
    }

    printf("[Z3BRA] EFI Driver found\n");

    int movement_type = std::stoi(config("movement_type"));

    bool triggerbot = stringToBool(config("triggerbot"));
    int triggerbot_delay_before_click = std::stoi(config("triggerbot_delay_before_click"));
    int triggerbot_delay_after_click = std::stoi(config("triggerbot_delay_after_click"));

    bool aimbot = stringToBool(config("aimbot"));
    int aimbot_fov = std::stoi(config("aimbot_fov"));
    int aimbot_speed = std::stoi(config("aimbot_speed"));
    int aimbot_smooth_amount = std::stoi(config("aimbot_smooth_amount"));

    printf("[Z3BRA] Config file found\n");


    auto hwnd = FindWindowA(NULL, "Counter-Strike 2");
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    if (!pid) {
        printf("[Z3BRA] CS2 not found\n");
        system("pause");
    }

    printf("[Z3BRA] CS2 found\n");
    uintptr_t client = getModuleAddress(pid, "client.dll");
    printf("[Z3BRA] client.dll base module address found\n");
    printf("[Z3BRA] Starting main thread...\n");


    if (movement_type == 1) {
        printf("[Z3BRA] Arduino movement in developing...\n");
        /*
        if (triggerbot) {
            TriggerbotArduino(pid, client, triggerbot_delay_before_click, triggerbot_delay_after_click);
        }
        */
    }
    else if (movement_type == 2) {
        if (triggerbot && aimbot) {
            std::thread triggerbotThread(TriggerbotMouse, pid, client, triggerbot_delay_before_click, triggerbot_delay_after_click);
            std::thread aimbotThread(AimbotMouse, pid, client, aimbot_speed, aimbot_smooth_amount, aimbot_fov);
            triggerbotThread.join();
            aimbotThread.join();
        }
        else if (aimbot) {
            std::thread aimbotThread(AimbotMouse, pid, client, aimbot_speed, aimbot_smooth_amount, aimbot_fov);
            aimbotThread.join();
        }
        else if (triggerbot) {
            std::thread triggerbotThread(TriggerbotMouse, pid, client, triggerbot_delay_before_click, triggerbot_delay_after_click);
            triggerbotThread.join();
        }

    }
    printf("[Z3BRA] All cheat features are turned off or problem with config file.\n");
    system("pause");
}
