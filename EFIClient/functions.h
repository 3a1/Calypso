#pragma once

#include "includes.h"
#include "struct.h"

DWORD_PTR getModuleAddress(DWORD processID, const char* moduleName);
bool CheckDriverStatus();
void LeftClick();
bool stringToBool(const std::string& str);