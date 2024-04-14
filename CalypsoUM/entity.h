#pragma once

#include "includes.h"
#include "struct.h"

class Entity
{
public:
	uintptr_t address;
	int health;
	int team;
	vector3 head;
};