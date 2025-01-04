#pragma once
#include "pch.h"

namespace dispatcher 
{
	uint64_t Start(command_t cmd);

	uint64_t CopyKernelMemory(uint64_t data[10]);
	uint64_t ReadProcessMemory(uint64_t data[10]);
	uint64_t WriteProcessMemory(uint64_t data[10]);
	uint64_t KillProcess(uint64_t data[10]);
	uint64_t PrivilegeEscalation(uint64_t data[10]);
}