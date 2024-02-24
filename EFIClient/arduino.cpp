#include "arduino.h"

bool arduino::scan_devices(LPCSTR device_name, LPSTR lp_out)
{
    bool status = false;
    char com[] = "COM";

    HDEVINFO device_info = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);
    if (device_info == INVALID_HANDLE_VALUE) { return false; }

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
            DWORD len = i + strlen(lp_pos);

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

bool arduino::send_data(char* buffer, DWORD buffer_size)
{
    DWORD bytes_written;
    return WriteFile(this->arduino_handle, buffer, buffer_size, &bytes_written, NULL);
}

arduino::arduino(LPCSTR device_name)
{
    char port[] = "\\.\\";

    while (!scan_devices(device_name, port))
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
            printf("GetCommState() failed\n");
            CloseHandle(this->arduino_handle);
        }

        dcb.BaudRate = CBR_9600;
        dcb.ByteSize = 8;
        dcb.StopBits = ONESTOPBIT;
        dcb.Parity = NOPARITY;
        if (!SetCommState(this->arduino_handle, &dcb))
        {
            printf("SetCommState() failed\n");
            CloseHandle(this->arduino_handle);
        }

        COMMTIMEOUTS cto = { 0 };
        cto.ReadIntervalTimeout = 50;
        cto.ReadTotalTimeoutConstant = 50;
        cto.ReadTotalTimeoutMultiplier = 10;
        cto.WriteTotalTimeoutConstant = 50;
        cto.WriteTotalTimeoutMultiplier = 10;
        if (!SetCommTimeouts(this->arduino_handle, &cto))
        {
            printf("SetCommTimeouts() failed\n");
            CloseHandle(this->arduino_handle);
        }
        printf("[Z] Connected to %s\n", device_name);
    }
}

arduino::~arduino()
{
    CloseHandle(this->arduino_handle);
}