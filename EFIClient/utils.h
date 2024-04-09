#pragma once
#include "includes.h"
#include "loop.h"
#include "struct.h"
#include "arduino.h"
#include "config.h"
#include "driver.h"
#include "offsets.h"
#include "features.h"

namespace utils 
{
	bool init();
	DWORD_PTR getModuleAddress(const DWORD processID, const char* moduleName);
	bool checkDriverStatus();
	void leftClickArduino();
	void leftClick();
	bool stob(const std::string& str);
	void aimAtPos(float x, float y);
	void aimAtPosArduino(float x, float y);
	double distance(const int x1, const int y1, const int x2, const int y2);
	void printLogo();
	void printc(const char* prefix, const char* text, WORD color);
}