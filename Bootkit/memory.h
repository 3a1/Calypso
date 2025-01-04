#pragma once
#include "pch.h"

namespace memory
{
	uint64_t copy(uint64_t* source, uint64_t* destination, uint64_t size);
	uint64_t copy_wp(uint64_t* source, uint64_t* destination, uint64_t size);
	uint64_t compare(uint8_t* source1, uint8_t* source2, uint64_t size);

	uint64_t scan(uint64_t start_addr, uint64_t size, uint8_t* pattern, uint64_t pattern_size);
	uint64_t scan_ex(uint64_t start_addr, uint64_t size, uint8_t* pattern, uint64_t pattern_size, uint8_t* mask);

	uint64_t get_section_address(uint64_t base_addr, const char* section);
	uint32_t get_section_size(uint64_t base_addr, const char* section);

	uint64_t scan_section(uint64_t base_addr, const char* section, uint8_t* pattern, uint64_t pattern_size);
	uint64_t scan_section_ex(uint64_t base_addr, const char* section, uint8_t* pattern, uint64_t pattern_size, uint8_t* mask);

	uint64_t get_export_address(uint64_t base_addr, const char* function);
};