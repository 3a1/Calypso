#include "utils.h"

uint32_t string::wcscmp(const wchar_t* s1, const wchar_t* s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (int)(*s1) - (int)(*s2);
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

uint64_t utils::get_module(uint64_t list_entry, const wchar_t* module)
{
    uint64_t current_entry = *(uint64_t*)list_entry;

    while (current_entry != list_entry)
    {
        uint64_t module_name_addr = *(uint64_t*)((uint8_t*)current_entry + 0x58 /* ->BaseDllName */ + 0x8 /* ->Buffer */);

        if (module_name_addr)
        {
            if (string::wcscmp((wchar_t*)module_name_addr, module) == 0)
            {
                return current_entry;
            }
        }

        current_entry = *(uint64_t*)current_entry;
    }

    return 0;
}

uint64_t utils::get_module_base(uint64_t list_entry, const wchar_t* module)
{
    uint64_t current_entry = utils::get_module(list_entry, module);

    if (!current_entry)
    {
        return 0;
    }

    uint64_t module_base = *(uint64_t*)(uint8_t*)(current_entry + 0x30 /* ->DllBase */);

    return module_base;
}