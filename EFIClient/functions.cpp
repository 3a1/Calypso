#include "functions.h"

#include "includes.h"

#define M_PI 3.14159265358979323846264338327950288419716939937510
ULONG ww = GetSystemMetrics(SM_CXSCREEN);
ULONG wh = GetSystemMetrics(SM_CYSCREEN);

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

