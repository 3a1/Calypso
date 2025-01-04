#pragma once
#include "pch.h"

namespace trampoline 
{
	void Hook(uint64_t function, uint64_t hook, uint8_t* original_data);
	void Unhook(uint64_t function, uint8_t* original_data);
}
