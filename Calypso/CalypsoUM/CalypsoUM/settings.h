#pragma once

#include "includes.h"
#include "struct.h"

namespace shared 
{
	
	namespace game
	{
		extern DWORD pid;
		extern uintptr_t client;
		extern uintptr_t engine;
		extern vector2 screen_size;
	};

	namespace soundesp
	{
		extern bool detected;
		extern int delay;
		extern vector3 local_pos;
	}

	namespace triggetbot
	{

		extern bool onCrosshair;
	};

	namespace aimbot
	{

		extern bool enable_temp;
	};

}

namespace settings
{

	namespace arduino 
	{
		extern bool enable;
		extern std::string name;
	};

	namespace misc 
	{
		extern bool only_enemies;
		extern bool only_visible;
		extern bool tg_with_ab;
		extern int  bone;
	};

	namespace soundesp
	{
		extern bool enable;
		extern std::string mode;
		extern int distance;
		extern float frequency;
	};

	namespace triggerbot 
	{
		extern bool enable;
		extern int  key;
		extern int  delay_b;
		extern int  delay_a;
	};

	namespace aimbot 
	{
		extern bool  enable;
		extern int   key;
		extern int fov;
		extern float smooth;
	};

}