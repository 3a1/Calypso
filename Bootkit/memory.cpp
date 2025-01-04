#include "memory.h"

uint64_t memory::copy(uint64_t* source, uint64_t* destination, uint64_t size)
{
	uint8_t* src  = (uint8_t*)source;
	uint8_t* dest = (uint8_t*)destination;

	for (uint64_t i = 0; i < size; ++i)
	{
		dest[i] = src[i];
	}

	return 0;
}

uint64_t memory::copy_wp(uint64_t* source, uint64_t* destination, uint64_t size)
{
	__disable_wp();

	memory::copy(source, destination, size);

	__enable_wp();

	return 0;
}

uint64_t memory::compare(uint8_t* source1, uint8_t* source2, uint64_t size)
{
	for (uint64_t i = 0; i < size; ++i)
	{
		if (source1[i] != source2[i])
		{
			return 1;
		}
	}

	return 0;
}

/* may go by patttern size beyond bounds while copying, but gets the job done */
uint64_t memory::scan(uint64_t start_addr, uint64_t size, uint8_t* pattern, uint64_t pattern_size)
{
	for (uint64_t i = 0; i < size; ++i)
	{
		uint64_t current_address = start_addr + i;

		if (memory::compare(pattern, (uint8_t*)current_address, pattern_size) == 0)
		{
			return current_address;
		}
	}

	return 0;
}

uint64_t memory::scan_ex(uint64_t start_addr, uint64_t size, uint8_t* pattern, uint64_t pattern_size, uint8_t* mask)
{
	for (uint64_t i = 0; i < size; ++i)
	{
		uint64_t current_address = start_addr + i;

		bool found = true;

		for (uint64_t x = 0; x < pattern_size; x++)
		{
			uint8_t current_byte = *((uint8_t*)(current_address + x));

			if (mask[x] == 0xFF && current_byte != pattern[x])
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			return current_address;
		}
	}

	return 0;
}

uint64_t memory::get_section_address(uint64_t base_addr, const char* section)
{
	uint16_t dos_magic = *(uint16_t*)(uint8_t*)(base_addr + 0x0 /* ->e_magic */);
	if (dos_magic != 0x5a4d) 
	{
		return 0;
	}

	uint64_t pe_header = base_addr + *(uint32_t*)(uint8_t*)(base_addr + 0x3c /* ->e_elfanew */);

	uint16_t pe_signature = *(uint16_t*)(uint8_t*)(pe_header + 0x0 /* ->Signature */);
	if (pe_signature != 0x4550) 
	{
		return 0;
	}

	uint16_t number_of_sections   = *(uint16_t*)(uint8_t*)(pe_header + 0x4 /* ->FileHeader */ + 0x2 /* ->NumberOfSections */);
	uint16_t optional_header_size = *(uint16_t*)(uint8_t*)(pe_header + 0x4 /* ->FileHeader */ + 0x10 /* ->SizeOfOptionalHeader */);

	uint64_t first_section_header = pe_header + 0x18 /* ->OptionalHeader */ + optional_header_size;

	for (uint16_t i = 0; i < number_of_sections; ++i)
	{
		uint64_t current_section = first_section_header + (i * 0x28 /* sizeof(IMAGE_SECTION_HEADER) */);

		char section_name[9] = { 0 };
		memory::copy((uint64_t*)(uint8_t*)(current_section + 0x0 /* ->Name */), (uint64_t*)&section_name, 8);

		if (memory::compare((uint8_t*)&section_name, (uint8_t*)section, string::strlen(section)) == 0)
		{
			uint32_t section_address_offset = *(uint32_t*)(current_section + 0xC /* ->VirtualAddress */);
			uint64_t section_address = base_addr + section_address_offset;

			return section_address;
		}
	}

	return 0;
}

uint32_t memory::get_section_size(uint64_t base_addr, const char* section)
{
	uint16_t dos_magic = *(uint16_t*)(uint8_t*)(base_addr + 0x0 /* ->e_magic */);
	if (dos_magic != 0x5a4d)
	{
		return 0;
	}

	uint64_t pe_header = base_addr + *(uint32_t*)(uint8_t*)(base_addr + 0x3c /* ->e_elfanew */);

	uint16_t pe_signature = *(uint16_t*)(uint8_t*)(pe_header + 0x0 /* ->Signature */);
	if (pe_signature != 0x4550)
	{
		return 0;
	}

	uint16_t number_of_sections   = *(uint16_t*)(uint8_t*)(pe_header + 0x4 /* ->FileHeader */ + 0x2 /* ->NumberOfSections */);
	uint16_t optional_header_size = *(uint16_t*)(uint8_t*)(pe_header + 0x4 /* ->FileHeader */ + 0x10 /* ->SizeOfOptionalHeader */);

	uint64_t first_section_header = pe_header + 0x18 /* ->OptionalHeader */ + optional_header_size;

	for (uint16_t i = 0; i < number_of_sections; ++i)
	{
		uint64_t current_section = first_section_header + (i * 0x28 /* sizeof(IMAGE_SECTION_HEADER) */);

		char section_name[9] = { 0 };
		memory::copy((uint64_t*)(uint8_t*)(current_section + 0x0 /* ->Name */), (uint64_t*)&section_name, 8);

		if (memory::compare((uint8_t*)&section_name, (uint8_t*)section, string::strlen(section)) == 0)
		{
			uint32_t section_size = *(uint32_t*)(current_section + 0x10 /* ->SizeOfRawData */);

			return section_size;
		}
	}

	return 0;
}

uint64_t memory::get_export_address(uint64_t base_addr, const char* function)
{
	uint64_t pe_header = base_addr + *(uint32_t*)(uint8_t*)(base_addr + 0x3c /* ->e_elfanew */);

	uint64_t export_dir = base_addr + *(uint32_t*)(uint8_t*)(pe_header + 0x18 /* ->OptionalHeader */ + 0x70 /* DataDirectory[0] */ + 0x0 /* ->VirtualAddress */);

	uint32_t name_list_count = *(uint32_t*)(uint8_t*)(export_dir + 0x18 /* ->NumberOfNames */);

	uint64_t address_list       = base_addr + *(uint32_t*)(uint8_t*)(export_dir + 0x1c /* ->AddressOfFunctions */);
	uint64_t name_list          = base_addr + *(uint32_t*)(uint8_t*)(export_dir + 0x20 /* ->AddressOfNames */);
	uint64_t name_ordinals_list = base_addr + *(uint32_t*)(uint8_t*)(export_dir + 0x24 /* ->AddressOfNameOrdinals */);

	for (uint32_t i = 0; i < name_list_count; ++i)
	{
		uint64_t name_address_rva = name_list + (i * sizeof(uint32_t));
		uint64_t name_addr = base_addr + *(uint32_t*)name_address_rva;
		char* name = (char*)(name_addr);

		if (string::strcmp(name, function) == 0)
		{
			uint64_t ordinal_address = name_ordinals_list + (i * sizeof(uint16_t));
			uint16_t ordinal = *(uint16_t*)ordinal_address;

			uint64_t function_address_rva = address_list + (ordinal * sizeof(uint32_t));
			uint64_t function_address = base_addr + *(uint32_t*)function_address_rva;

			return function_address;
		}
	}
	
	return 0;
}

uint64_t memory::scan_section(uint64_t base_addr, const char* section, uint8_t* pattern, uint64_t pattern_size)
{
	uint64_t section_address = memory::get_section_address(base_addr, section);
	uint32_t section_size    = memory::get_section_size(base_addr, section);

	for (uint64_t i = 0; i < section_size; ++i)
	{
		uint64_t current_address = section_address + i;

		if (memory::compare(pattern, (uint8_t*)current_address, pattern_size) == 0)
		{
			return current_address;
		}
	}

	return 0;
}

uint64_t memory::scan_section_ex(uint64_t base_addr, const char* section, uint8_t* pattern, uint64_t pattern_size, uint8_t* mask)
{
	uint64_t section_address = memory::get_section_address(base_addr, section);
	uint32_t section_size    = memory::get_section_size(base_addr, section);

	for (uint64_t i = 0; i < section_size; ++i)
	{
		uint64_t current_address = section_address + i;

		bool found = true;

		for (uint64_t x = 0; x < pattern_size; x++)
		{
			uint8_t current_byte = *((uint8_t*)(current_address + x));

			if (mask[x] == 0xFF && current_byte != pattern[x])
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			return current_address;
		}
	}

	return 0;
}

/* sometimes, taking a leap forward... means leaving a few behind */