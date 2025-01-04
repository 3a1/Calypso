#include "utils.h"

uint32_t string::wcscmp(const wchar_t* s1, const wchar_t* s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(unsigned int*)s1 - *(unsigned int*)s2;
}

uint32_t string::strcmp(const char* s1, const char* s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

uint64_t string::strlen(const char* str) 
{
    const char* s = str;
    while (*s++) {}
    return s - str - 1;
}

void string::AsciiToUnicode(const char* ascii, CHAR16* unicode)
{
    if (!ascii || !unicode)
        return;

    while (*ascii)
        *unicode++ = (CHAR16)(*ascii++);

    *unicode = L'\0'; // Null-terminate the Unicode string
}

uint64_t utils::get_winload_base(uint64_t address)
{
	address = address & ~0xFFF;

	do {
		uint32_t value = *(uint32_t*)address;

		if (value == 0x905a4d)
		{
			return address;
		}

		address -= 0x1000;
	} while (address != 0);

	return address;
}

uint64_t utils::get_module_base(uint64_t loader_block_addr, const wchar_t* module)
{
    uint64_t current_entry = 0;
    memory::copy((uint64_t*)((uint8_t*)loader_block_addr + 0x10 /* ->LoadOrderListHead */), &current_entry, sizeof(current_entry));

    while (current_entry != (uint64_t)((uint8_t*)loader_block_addr + 0x10 /* ->LoadOrderListHead */))
    {
        uint64_t module_base = 0;
        memory::copy((uint64_t*)((uint8_t*)current_entry + 0x30 /* ->DllBase */), &module_base, sizeof(uint64_t));

        uint64_t module_name_addr = 0;
        memory::copy((uint64_t*)((uint8_t*)current_entry + 0x58 /* ->BaseDllName */ + 0x8 /* ->Buffer */), &module_name_addr, sizeof(uint64_t));

        wchar_t module_name[256] = { 0 };
        memory::copy((uint64_t*)module_name_addr, (uint64_t*)&module_name, sizeof(module_name));

        if (module_name)
        {
            if (string::wcscmp(module_name, module) == 0)
            {
                return module_base;
            }
        }

        memory::copy((uint64_t*)current_entry, &current_entry, sizeof(current_entry));
    }

    return 0;
}