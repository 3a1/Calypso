#include "arduino.h"

Arduino Arduino::arduino;

bool Arduino::scan(LPCSTR device_name, LPSTR lp_out)
{
    bool status = false;
    char com[] = "COM";

    HDEVINFO device_info = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);
    if (device_info == INVALID_HANDLE_VALUE) return false;

    SP_DEVINFO_DATA dev_info_data;
    dev_info_data.cbSize = sizeof(dev_info_data);

    DWORD count = 0;

    while (SetupDiEnumDeviceInfo(device_info, count++, &dev_info_data))
    {
        BYTE buffer[256];
        if (SetupDiGetDeviceRegistryProperty(device_info, &dev_info_data, SPDRP_FRIENDLYNAME, NULL, buffer, sizeof(buffer), NULL))
        {
            DWORD i = strlen(lp_out);
            LPCSTR lp_pos = strstr((LPCSTR)buffer, com);
            DWORD len = i + (lp_pos ? strlen(lp_pos) : 0);

            if (strstr((LPCSTR)buffer, device_name) && lp_pos)
            {
                for (DWORD j = 0; i < len; i++, j++)
                {
                    lp_out[i] = lp_pos[j];
                }

                lp_out[i - 1] = '\0';
                status = true;
                break;
            }
        }
    }

    SetupDiDestroyDeviceInfoList(device_info);
    return status;
}

bool Arduino::send(char* buffer, DWORD buffer_size)
{
    DWORD bytes_written;
    return WriteFile(this->arduino_handle, buffer, buffer_size, &bytes_written, NULL);
}

bool Arduino::init()
{
    char port[] = "\\.\\";
    bool error = false;

    Log("[+]", "Waiting for arduino [...]", GREEN);

    while (!scan(settings::arduino::name.c_str(), port))
    { 
        Sleep(1000);
    }

    this->arduino_handle = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (this->arduino_handle)
    {
        DCB dcb = { 0 };
        dcb.DCBlength = sizeof(dcb);
        if (!GetCommState(this->arduino_handle, &dcb))
        {
            Log("[-]", "GetCommState() failed", RED);
            CloseHandle(this->arduino_handle);
            error = true;
        }

        dcb.BaudRate = CBR_9600;
        dcb.ByteSize = 8;
        dcb.StopBits = ONESTOPBIT;
        dcb.Parity = NOPARITY;
        if (!SetCommState(this->arduino_handle, &dcb))
        {
            Log("[-]", "SetCommState() failed", RED);
            CloseHandle(this->arduino_handle);
            error = true;
        }

        COMMTIMEOUTS cto = { 0 };
        cto.ReadIntervalTimeout = 50;
        cto.ReadTotalTimeoutConstant = 50;
        cto.ReadTotalTimeoutMultiplier = 10;
        cto.WriteTotalTimeoutConstant = 50;
        cto.WriteTotalTimeoutMultiplier = 10;
        if (!SetCommTimeouts(this->arduino_handle, &cto))
        {
            Log("[-]", "SetCommTimeouts() failed", RED);
            CloseHandle(this->arduino_handle);
            error = true;
        }
        if (error)
        {
            Log("\n[!]", "There are errors with arduino connection", YELLOW);
            Log("[!]", "Arduino can doesn't work when security boot on/TPM on/Windows11", YELLOW);
            Log("[!]", "If still doesn't work try use Arduino IDE 1.8.9 to update .ino", YELLOW);
            Log("[!]", "Try to:", YELLOW);
            Log("   [!]", "Restart cheat", YELLOW);
            Log("   [!]", "Change arduino port", YELLOW);
            Log("   [!]", "Reupload arduino scratch\n", YELLOW);
            system("pause");
            exit(0);
            return false;
        }
        else 
        {
            Log("[+]", "Connected to " + settings::arduino::name, GREEN);
            return true;
        }
    }
}

Arduino::~Arduino()
{
    CloseHandle(this->arduino_handle);
}