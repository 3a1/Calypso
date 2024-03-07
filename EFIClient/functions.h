#pragma once

#include "includes.h"
#include "struct.h"

DWORD_PTR getModuleAddress(DWORD processID, const char* moduleName);
bool CheckDriverStatus();
void LeftClickArduino();
void LeftClick();
bool stringToBool(const std::string& str);
void AimAtPos(float x, float y, int AimSpeed, int SmoothAmount);
void AimAtPosArduino(float x, float y, int AimSpeed, int SmoothAmount);
double distance(int x1, int y1, int x2, int y2);