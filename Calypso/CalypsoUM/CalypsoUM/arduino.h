#pragma once
#include "includes.h"
#include "utils.h"

class Arduino
{
private:
	HANDLE arduino_handle = NULL;

	bool scan(LPCSTR device_name, LPSTR lp_out);

public:

	static Arduino arduino;

	bool send(char* buffer, DWORD buffer_size);

	bool init();

	~Arduino();

};