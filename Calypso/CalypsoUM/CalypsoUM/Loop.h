#pragma once
#include "includes.h"
#include "struct.h"
#include "settings.h"
#include "driver.h"
#include "utils.h"
#include "offsets.h"

enum SIGNONSTATE
{
	SIGNONSTATE_NONE = 0,
	SIGNONSTATE_CHALLENGE = 1,
	SIGNONSTATE_CONNECTED = 2,
	SIGNONSTATE_NEW = 3,
	SIGNONSTATE_PRESPAWN = 4,
	SIGNONSTATE_SPAWN = 5,
	SIGNONSTATE_FULL = 6,
	SIGNONSTATE_CHANGELEVEL = 7,
};

struct Player
{
	vector2 position;
};

struct Origin
{
	vector3 origin;
};


namespace entity_loop
{
	extern std::vector<Player> player_list;
	extern std::vector<Origin> origin_list;

	bool start();

	void loop();

};