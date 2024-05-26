#pragma once
#include "includes.h"

#include "settings.h"
#include "arduino.h"
#include "keys.h"

#define Log(prefix, text, color) utils::output::printc(prefix, text, color)
#define Logo() utils::output::printl()


namespace utils 
{

	namespace process 
	{
		DWORD findProcessIdByName(const std::string& processName);
		DWORD_PTR getModuleAddress(const DWORD processID, const char* moduleName);
	}

	namespace mouse 
	{
		bool left_click();
		void aim_at(float x, float y);
	}

	namespace audio 
	{
		void beep();
	}

	namespace text 
	{
		std::string utos(const std::string& str); // underscores to spaces
		bool stob(const std::string& str); // string to bool
		int	 stovk(const std::string& str); // string to virtual key
		int  stobn(const std::string& boneName); // string to bone
	}

	namespace math 
	{
		double distance(const int x1, const int y1, const int x2, const int y2);
	}

	namespace output 
	{
		void printl();
		void printc(const char* prefix, const std::string& text, WORD color);
	}
}