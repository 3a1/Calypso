#pragma once

#include <Windows.h>
#include <iostream>
#include <devguid.h>
#include <SetupAPI.h>

#pragma comment (lib, "Setupapi.lib")

class arduino
{
private:

	HANDLE arduino_handle = NULL;

public:

	bool scan_devices(LPCSTR device_name, LPSTR lp_out);

	bool send_data(char* buffer, DWORD buffer_size);

	arduino(LPCSTR device_name);

	~arduino();

};