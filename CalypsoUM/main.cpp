#include "main.h"

bool init() {
    SetConsoleTitle("CS2-Calypso");
    Logo();

    if (!driver::init() || !driver::checkDriverStatus())
    {
        UNICODE_STRING VariableName = RTL_CONSTANT_STRING(VARIABLE_NAME);
        NtSetSystemEnvironmentValueEx(
            &VariableName,
            &DummyGuid,
            0,
            0,
            ATTRIBUTES);

        Log("[-]", "No EFI Driver found", RED);
        system("pause");
        exit(1);
    }
    else
    {
        Log("[+]", "EFI Driver found", GREEN);
    }

    Config::init();

    if (settings::arduino::enable)
        Arduino::arduino.init();


    shared::game::pid = utils::process::findProcessIdByName("cs2.exe");
    if (!shared::game::pid || shared::game::pid == 1)
    {
        Log("[-]", "CS2 not found", RED);
        system("pause");
        exit(1);
    }
    else
    {
        Log("[+]", "CS2 found", GREEN);
    }

    shared::game::client = utils::process::getModuleAddress(shared::game::pid, "client.dll");
    if (!shared::game::client || shared::game::client == 1)
    {
        Log("[-]", "client.dll address not found", RED);
        system("pause");
        exit(1);
    }
    else
    {
        std::ostringstream oss;
        oss << std::hex << shared::game::client;
        Log("[+]", "client.dll address found -> 0x" + oss.str(), GREEN);
    }

    shared::game::engine = utils::process::getModuleAddress(shared::game::pid, "engine2.dll");
    if (!shared::game::engine || shared::game::engine == 1)
    {
        Log("[-]", "engine2.dll address not found", RED);
        system("pause");
        exit(1);
    }
    else
    {
        std::ostringstream oss;
        oss << std::hex << shared::game::engine;
        Log("[+]", "engine.dll address found -> 0x" + oss.str(), GREEN);
    }

    if (loop::start()) 
    {
        Log("[+]", "Entity loop thread started", GREEN);
    }
    else 
    {
        Log("[-]", "Failed to start entity loop thread", RED);
    }

    if (!features::start()) 
    {
        Log("[-]", "All cheat features are turned off or problem with config file.", RED);
        system("pause");
        return false;
    }

    return true;
}

int main()
{
    if (init()) 
    {
        for (;;) 
        {
            //if (GetAsyncKeyState(settings::aimbot::key) || shared::aimbot::enable_temp)
            std::this_thread::sleep_for(std::chrono::seconds(999));
        }
    }
}