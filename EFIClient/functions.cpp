#include "functions.h"

#include "includes.h"
#include "arduino.h"

#define M_PI 3.14159265358979323846264338327950288419716939937510
ULONG ww = GetSystemMetrics(SM_CXSCREEN);
ULONG wh = GetSystemMetrics(SM_CYSCREEN);

arduino duino;

DWORD_PTR getModuleAddress(DWORD processID, const char* moduleName)
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

double distance(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}


bool CheckDriverStatus() {
    int icheck = 82;
    NTSTATUS status = 0;

    uintptr_t BaseAddr = Driver::GetBaseAddress(GetCurrentProcessId());
    if (BaseAddr == 0) {
        return false;
    }

    int checked = Driver::read<int>(GetCurrentProcessId(), (uintptr_t)&icheck, &status);
    if (checked != icheck) {
        return false;
    }
    return true;
}

void LeftClick()
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

void LeftClickArduino()
{
    char buffer[] = "0:0;1"; // Make sure the buffer is large enough to hold the formatted string
    duino.send_data(buffer, sizeof(buffer));
}

bool stringToBool(const std::string& str) {
    return str == "true";
}

void AimAtPos(float x, float y, int AimSpeed, int SmoothAmount)
{
    float TargetX = 0;
    float TargetY = 0;

    POINT cursorPos;
    GetCursorPos(&cursorPos);

    int ScreenCenterX = cursorPos.x;
    int ScreenCenterY = cursorPos.y;

    bool Smooth = true;

    if (AimSpeed == 0) AimSpeed = 1;
    if (SmoothAmount == 0) SmoothAmount = 1;

    if (x != 0)
    {
        if (x > ScreenCenterX)
        {
            TargetX = -(ScreenCenterX - x);
            TargetX /= AimSpeed;
            if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
        }

        if (x < ScreenCenterX)
        {
            TargetX = x - ScreenCenterX;
            TargetX /= AimSpeed;
            if (TargetX + ScreenCenterX < 0) TargetX = 0;
        }
    }

    if (y != 0)
    {
        if (y > ScreenCenterY)
        {
            TargetY = -(ScreenCenterY - y);
            TargetY /= AimSpeed;
            if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
        }

        if (y < ScreenCenterY)
        {
            TargetY = y - ScreenCenterY;
            TargetY /= AimSpeed;
            if (TargetY + ScreenCenterY < 0) TargetY = 0;
        }
    }

    if (!Smooth)
    {
        mouse_event(0x0001, (UINT)(TargetX), (UINT)(TargetY), NULL, NULL);
        return;
    }
    else
    {
        TargetX /= SmoothAmount;
        TargetY /= SmoothAmount;
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
}

void AimAtPosArduino(float x, float y, int AimSpeed, int SmoothAmount)
{

    float TargetX = 0;
    float TargetY = 0;

    POINT cursorPos;
    GetCursorPos(&cursorPos);

    int ScreenCenterX = cursorPos.x;
    int ScreenCenterY = cursorPos.y;

    bool Smooth = true;

    if (AimSpeed == 0) AimSpeed = 1;
    if (SmoothAmount == 0) SmoothAmount = 1;

    if (x != 0)
    {
        if (x > ScreenCenterX)
        {
            TargetX = -(ScreenCenterX - x);
            TargetX /= AimSpeed;
            if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
        }

        if (x < ScreenCenterX)
        {
            TargetX = x - ScreenCenterX;
            TargetX /= AimSpeed;
            if (TargetX + ScreenCenterX < 0) TargetX = 0;
        }
    }

    if (y != 0)
    {
        if (y > ScreenCenterY)
        {
            TargetY = -(ScreenCenterY - y);
            TargetY /= AimSpeed;
            if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
        }

        if (y < ScreenCenterY)
        {
            TargetY = y - ScreenCenterY;
            TargetY /= AimSpeed;
            if (TargetY + ScreenCenterY < 0) TargetY = 0;
        }
    }

    if (!Smooth)
    {
        mouse_event(0x0001, (UINT)(TargetX), (UINT)(TargetY), NULL, NULL);
        return;
    }
    else
    {
        TargetX /= SmoothAmount;
        TargetY /= SmoothAmount;
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
        duino.send_data(buffer, sizeof(buffer));
        return;
    }
}