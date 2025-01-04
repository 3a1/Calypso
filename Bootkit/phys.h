#pragma once
#include "pch.h"

uint64_t get_process_dtb(uint64_t pid);
uint64_t read_phys_mmu(uint64_t address);
uint64_t va_to_pa(uint64_t va, uint64_t pid);

uint64_t ReadPhysicalMemory(uint64_t address, uint64_t* buffer, uint64_t size);
uint64_t WritePhysicalMemory(uint64_t address, uint64_t* buffer, uint64_t size);