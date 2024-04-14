#include "settings.h"

namespace shared 
{

	namespace game
	{
		DWORD pid = 0;
		uintptr_t client = 0;
		uintptr_t engine = 0;
		vector2 screen_size = { 0, 0 };
	};

	namespace triggetbot
	{

		bool onCrosshair = false;
	}

	namespace aimbot
	{

		bool enable_temp = false;
	}

}

namespace settings
{

	namespace arduino
	{
		bool enable = false;
		std::string name = "";
	};

	namespace misc
	{
		bool only_enemies = false;
		bool tg_with_ab = false;
	};

	namespace triggerbot
	{
		bool enable = false;
		int key = 0;
		int delay_b = 0;
		int delay_a = 0;
	};

	namespace aimbot
	{
		bool enable = false;
		int key = 0;
		float fov = 0.0f;
		int speed = 0;
		int smooth = 0;
	};

}