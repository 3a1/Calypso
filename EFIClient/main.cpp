

#include "includes.h"
#include "offsets.h"
#include "functions.h"
#include <stdint.h>
#include "arduino.h"
#include "config.h"
#include "mainLoop.h"
#include "features.h"

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

        printf("No EFI Driver found\n");
        system("pause");
        exit(1);
        return 1;
    }

    printf("[Z3BRA] EFI Driver found\n");

    int movement_type = std::stoi(config("movement_type"));
    bool only_enemies = stringToBool(config("only_enemies"));
    int head_position = std::stoi(config("head_position"));

    bool triggerbot = stringToBool(config("triggerbot"));
    int triggerbot_button = std::stoi(config("triggerbot_button"));
    int triggerbot_delay_b = std::stoi(config("triggerbot_delay_before_click"));
    int triggerbot_delay_a = std::stoi(config("triggerbot_delay_after_click"));

    bool aimbot = stringToBool(config("aimbot"));
    int aimbot_button = std::stoi(config("aimbot_button"));
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

    Loop loopInstance(pid, client, triggerbot, only_enemies, head_position);
    std::thread updateThread(&Loop::updateLoop, &loopInstance);
    updateThread.detach();

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    
    if (movement_type == 1) {
        printf("[Z3BRA] Arduino movement in developing...\n");
        
    }
    else if (movement_type == 2) {
        if (triggerbot && aimbot) {
            std::thread triggerbotThread(mouseTriggerbot, std::ref(loopInstance), triggerbot_delay_b, triggerbot_delay_a, triggerbot_button);
            std::thread aimbotThread(mouseAimbot, std::ref(loopInstance), aimbot_speed, aimbot_smooth_amount, aimbot_fov, aimbot_button);
            aimbotThread.join();
        }
        else if (aimbot) {
            std::thread aimbotThread(mouseAimbot, std::ref(loopInstance), aimbot_speed, aimbot_smooth_amount, aimbot_fov, aimbot_button);
            aimbotThread.join();
        }
        else if (triggerbot) {
            std::thread triggerbotThread(mouseTriggerbot, std::ref(loopInstance), triggerbot_delay_b, triggerbot_delay_a, triggerbot_button);
            triggerbotThread.join();
        }

    }
    
    printf("[Z3BRA] All cheat features are turned off or problem with config file.\n");
    system("pause");
}
