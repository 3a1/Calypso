#pragma once

#include "includes.h"
#include "struct.h"

DWORD_PTR getModuleAddress(DWORD processID, const char* moduleName);
bool CheckDriverStatus();