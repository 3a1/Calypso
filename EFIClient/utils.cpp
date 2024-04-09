#include "utils.h"

bool utils::init() {
    SetConsoleTitle("CS2-UEFI-Arduino");
    utils::printLogo();

    if (!driver::init() || !utils::checkDriverStatus())
    {
        UNICODE_STRING VariableName = RTL_CONSTANT_STRING(VARIABLE_NAME);
        NtSetSystemEnvironmentValueEx(
            &VariableName,
            &DummyGuid,
            0,
            0,
            ATTRIBUTES);

        utils::printc("[-]", "No EFI Driver found", RED);
        system("pause");
        exit(1);
    }
    else 
    {
        utils::printc("[+]", "EFI Driver found", GREEN);
    }

    config::readConfigFile();

    if (config::arduino.enable) 
        Arduino::arduino.initialize(config::arduino.name.c_str());

    auto hwnd = FindWindowA(NULL, "Counter-Strike 2");
    GetWindowThreadProcessId(hwnd, &Loop::loop.pid);
    if (!Loop::loop.pid || Loop::loop.pid == 1)
    {
        utils::printc("[-]", "CS2 not found", RED);
        system("pause");
        exit(1);
    }
    else 
    {
        utils::printc("[+]", "CS2 found", GREEN);
    }

    Loop::loop.client = utils::getModuleAddress(Loop::loop.pid, "client.dll");
    if (!Loop::loop.client || Loop::loop.client == 1)
    {
        utils::printc("[-]", "client.dll address not found", RED);
        system("pause");
        exit(1);
    }
    else
    {
        utils::printc("[+]", "client.dll address found", GREEN);
    }

    std::thread updateThread([]() { Loop::loop.updateLoop(); });

    if (updateThread.joinable()) {
        utils::printc("[+]", "Entity loop thread started", GREEN);
        updateThread.detach();
    }
    else {
        utils::printc("[-]", "Failed to start entity loop thread", RED);
    }

    features::start();

    utils::printc("[-]", "All cheat features are turned off or problem with config file.", RED);
    system("pause");
    return false;
}

DWORD_PTR utils::getModuleAddress(const DWORD processID, const char* moduleName)
{
    DWORD_PTR dwModuleBaseAddress = 0;
    DWORD_PTR result = 0;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 ModuleEntry32;
        ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hSnapshot, &ModuleEntry32))
        {
            do {
                if (_stricmp(ModuleEntry32.szModule, moduleName) == 0)
                {
                    dwModuleBaseAddress = reinterpret_cast<DWORD_PTR>(ModuleEntry32.modBaseAddr);
                    result = dwModuleBaseAddress;
                    break;
                }
            } while (Module32Next(hSnapshot, &ModuleEntry32));
        }
        CloseHandle(hSnapshot);
    }
    return result;
}

double utils::distance(
    const int x1,
    const int y1,
    const int x2,
    const int y2) 
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}


bool utils::checkDriverStatus() 
{
    int icheck = 82;
    NTSTATUS status = 0;

    uintptr_t BaseAddr = driver::getBaseAddress(GetCurrentProcessId());
    if (BaseAddr == 0) 
    {
        return false;
    }

    int checked = driver::read<int>(GetCurrentProcessId(), (uintptr_t)&icheck, &status);
    if (checked != icheck) 
    {
        return false;
    }
    return true;
}

void utils::leftClick()
{
    INPUT    Input = { 0 };
    // left down 
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));

    // left up
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

void utils::leftClickArduino()
{
    char buffer[] = "0:0;1";
    Arduino::arduino.send_data(buffer, sizeof(buffer));
}

bool utils::stob(const std::string& str) 
{
    if (str == "true" || str == "1") 
    {
        return true;
    }
    else if (str == "false" || str == "0") 
    {
        return false;
    }
}

void utils::aimAtPos(float x, float y)
{
    float TargetX = 0;
    float TargetY = 0;

    POINT cursorPos;
    GetCursorPos(&cursorPos);

    int ScreenCenterX = cursorPos.x;
    int ScreenCenterY = cursorPos.y;

    if (config::aimbot.speed == 0) config::aimbot.speed = 1;
    if (config::aimbot.smooth == 0) config::aimbot.smooth = 1;

    if (x != 0)
    {
        if (x > ScreenCenterX)
        {
            TargetX = -(ScreenCenterX - x);
            TargetX /= config::aimbot.speed;
            if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
        }

        if (x < ScreenCenterX)
        {
            TargetX = x - ScreenCenterX;
            TargetX /= config::aimbot.speed;
            if (TargetX + ScreenCenterX < 0) TargetX = 0;
        }
    }

    if (y != 0)
    {
        if (y > ScreenCenterY)
        {
            TargetY = -(ScreenCenterY - y);
            TargetY /= config::aimbot.speed;
            if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
        }

        if (y < ScreenCenterY)
        {
            TargetY = y - ScreenCenterY;
            TargetY /= config::aimbot.speed;
            if (TargetY + ScreenCenterY < 0) TargetY = 0;
        }
    }

    TargetX /= config::aimbot.smooth;
    TargetY /= config::aimbot.smooth;
    if (abs(TargetX) < 1)
    {
        if (TargetX > 0)
        {
            TargetX = 1;
        }
        if (TargetX < 0)
        {
            TargetX = -1;
        }
    }
    if (abs(TargetY) < 1)
    {
        if (TargetY > 0)
        {
            TargetY = 1;
        }
        if (TargetY < 0)
        {
            TargetY = -1;
        }
    }
    mouse_event(0x0001, (UINT)(TargetX), (UINT)(TargetY), NULL, NULL);
    return;
}

void utils::aimAtPosArduino(float x, float y)
{
    float TargetX = 0;
    float TargetY = 0;

    POINT cursorPos;
    GetCursorPos(&cursorPos);

    int ScreenCenterX = cursorPos.x;
    int ScreenCenterY = cursorPos.y;

    if (config::aimbot.speed == 0) config::aimbot.speed = 1;
    if (config::aimbot.smooth == 0) config::aimbot.smooth = 1;

    if (x != 0)
    {
        if (x > ScreenCenterX)
        {
            TargetX = -(ScreenCenterX - x);
            TargetX /= config::aimbot.speed;
            if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
        }

        if (x < ScreenCenterX)
        {
            TargetX = x - ScreenCenterX;
            TargetX /= config::aimbot.speed;
            if (TargetX + ScreenCenterX < 0) TargetX = 0;
        }
    }

    if (y != 0)
    {
        if (y > ScreenCenterY)
        {
            TargetY = -(ScreenCenterY - y);
            TargetY /= config::aimbot.speed;
            if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
        }

        if (y < ScreenCenterY)
        {
            TargetY = y - ScreenCenterY;
            TargetY /= config::aimbot.speed;
            if (TargetY + ScreenCenterY < 0) TargetY = 0;
        }
    }

    TargetX /= config::aimbot.smooth;
    TargetY /= config::aimbot.smooth;
    if (abs(TargetX) < 1)
    {
        if (TargetX > 0)
        {
            TargetX = 1;
        }
        if (TargetX < 0)
        {
            TargetX = -1;
        }
    }
    if (abs(TargetY) < 1)
    {
        if (TargetY > 0)
        {
            TargetY = 1;
        }
        if (TargetY < 0)
        {
            TargetY = -1;
        }
    }

    char buffer[20];
    sprintf_s(buffer, "%d:%d;0", (int)TargetX, (int)TargetY);
    Arduino::arduino.send_data(buffer, sizeof(buffer));
    return;
    
}

void utils::printLogo() 
{
    printf("  _________             \n");
    printf(" |_  /__ / |__ _ _ __ _ \n");
    printf("  / / |_ \\ '_ \\ '_/ _` |\n");
    printf(" /___|___/_.__/_| \\__,_| \n\n");
    return;
}

void utils::printc(const char* prefix, const char* text, WORD color)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, color);
    printf("%s ", prefix);

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("%s\n", text);
}