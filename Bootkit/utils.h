#pragma once
#include "pch.h"

namespace utils
{
	uint64_t get_winload_base(uint64_t address);
	uint64_t get_module_base(uint64_t loader_block_addr, const wchar_t* module);
};

namespace string
{
	uint32_t strcmp(const char* s1, const char* s2);
	uint32_t wcscmp(const wchar_t* s1, const wchar_t* s2);
	uint64_t strlen(const char* str);

	void AsciiToUnicode(const char* ascii, CHAR16* unicode);
};