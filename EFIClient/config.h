#pragma once
#include "includes.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <algorithm>

namespace config
{
	struct Arduino {
		bool enable;
		std::string name;
	};

	struct Misc {
		bool only_enemies;
		float head_position;
		bool tg_with_ab;
	};

	struct Triggerbot {
		bool enable;
		int key;
		int delay_b;
		int delay_a;
	};

	struct Aimbot {
		bool enable;
		int key;
		float fov;
		int speed;
		int smooth;
		bool enable_b = false;
	};

	extern Arduino arduino;
	extern Misc misc;
	extern Triggerbot triggerbot;
	extern Aimbot aimbot;

    void readConfigFile();
    void createConfigFile();
	std::string config(std::string section, std::string param);
}