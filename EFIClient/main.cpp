#include "includes.h"
#include "offsets.h"
#include "functions.h"
#include <stdint.h>
#include "arduino.h"
#include "config.h"
#include "mainLoop.h"
#include "features.h"

extern arduino duino;

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

    std::string arduino_name ="Arduino " + config("arduino");

    int movement_type = std::stoi(config("movement-type"));
    bool only_enemies = stringToBool(config("only-enemies"));
    int head_position = std::stoi(config("head-position"));

    bool triggerbot = stringToBool(config("triggerbot"));
    int triggerbot_button = std::stoi(config("triggerbot-key"));
    int triggerbot_delay_b = std::stoi(config("triggerbot-delay-before-click"));
    int triggerbot_delay_a = std::stoi(config("triggerbot-delay-after-click"));

    bool aimbot = stringToBool(config("aimbot"));
    int aimbot_button = std::stoi(config("aimbot-key"));
    int aimbot_fov = std::stoi(config("aimbot-fov"));
    int aimbot_speed = std::stoi(config("aimbot-speed"));
    int aimbot_smooth_amount = std::stoi(config("aimbot-smooth"));

    printf("[Z3BRA] Config file found\n");

    if (movement_type == 1) {
        printf("[Z3BRA] Waiting for arduino...\n");
        duino.initialize(arduino_name.c_str());
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

    printf("[Z3BRA] Starting main thread...\n");

    Loop loopInstance(pid, client, triggerbot, only_enemies, head_position);
    std::thread updateThread(&Loop::updateLoop, &loopInstance);
    updateThread.detach();

    if (movement_type == 1) {
        if (triggerbot && aimbot) {
            std::thread triggerbotThread(Arduino::Triggerbot::start, std::ref(loopInstance), triggerbot_delay_b, triggerbot_delay_a, triggerbot_button);
            std::thread aimbotThread(Arduino::Aimbot::start, std::ref(loopInstance), aimbot_speed, aimbot_smooth_amount, aimbot_fov, aimbot_button);
            aimbotThread.join();

        }
        else if (aimbot) {
            std::thread aimbotThread(Arduino::Aimbot::start, std::ref(loopInstance), aimbot_speed, aimbot_smooth_amount, aimbot_fov, aimbot_button);
            aimbotThread.join();

        }
        else if (triggerbot) {
            std::thread triggerbotThread(Arduino::Triggerbot::start, std::ref(loopInstance), triggerbot_delay_b, triggerbot_delay_a, triggerbot_button);
            triggerbotThread.join();
        }
        
    } else if (movement_type == 2) {
        if (triggerbot && aimbot) {
            std::thread triggerbotThread(Mouse::Triggerbot::start, std::ref(loopInstance), triggerbot_delay_b, triggerbot_delay_a, triggerbot_button);
            std::thread aimbotThread(Mouse::Aimbot::start, std::ref(loopInstance), aimbot_speed, aimbot_smooth_amount, aimbot_fov, aimbot_button);
            aimbotThread.join();

        } else if (aimbot) {
            std::thread aimbotThread(Mouse::Aimbot::start, std::ref(loopInstance), aimbot_speed, aimbot_smooth_amount, aimbot_fov, aimbot_button);
            aimbotThread.join();

        } else if (triggerbot) {
            std::thread triggerbotThread(Mouse::Triggerbot::start, std::ref(loopInstance), triggerbot_delay_b, triggerbot_delay_a, triggerbot_button);
            triggerbotThread.join();
        }
    }
    
    printf("[Z3BRA] All cheat features are turned off or problem with config file.\n");
    system("pause");
}
