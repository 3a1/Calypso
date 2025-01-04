#include "trampoline.h"

void trampoline::Hook(uint64_t function, uint64_t hook, uint8_t* original_data)
{
	uint8_t trampoline[] = 
	{
		0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* movabs rax, <address> */
		0xFF, 0xE0													/* jmp rax				 */
	};
	memory::copy(&hook, (uint64_t*)((uint8_t*)trampoline + 2), sizeof(uint64_t));

	memory::copy((uint64_t*)function, (uint64_t*)original_data, TRAMPOLINE_SIZE);

	memory::copy_wp((uint64_t*)trampoline, (uint64_t*)function, TRAMPOLINE_SIZE);
}

void trampoline::Unhook(uint64_t function, uint8_t* original_data)
{
	memory::copy_wp((uint64_t*)original_data, (uint64_t*)function, TRAMPOLINE_SIZE);
}