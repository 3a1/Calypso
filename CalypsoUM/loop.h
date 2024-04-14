#pragma once
#include "includes.h"
#include "struct.h"
#include "entity.h"
#include "settings.h"
#include "driver.h"
#include "utils.h"

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

namespace loop
{
	extern std::vector<Player> player_list;

	bool start();

	void cacheLoop();
	bool update_entity(Entity* entity);
	bool triggetbot(Entity* localPlayer, uintptr_t* entity_list);
};