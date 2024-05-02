#pragma once

#define RED (FOREGROUND_RED)
#define GREEN (FOREGROUND_GREEN)
#define YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)

#pragma comment(lib, "ntdll.lib")
#pragma comment (lib, "Setupapi.lib")

#include <windows.h>
#include <sstream>
#include <string>
#include <iostream>
#include <Tlhelp32.h>
#include <winternl.h>
#include <fstream>
#include <vector>
#include <SetupAPI.h>
#include <devguid.h>
#include <chrono>
#include <thread>
#include <future>
#include <unordered_map>
#include <map>
