#include "phys.h"

uint64_t get_process_dtb(uint64_t pid)
{
	uint64_t PsLookupProcessByProcessId = memory::get_export_address(global::ntoskrnl, "PsLookupProcessByProcessId");

	uint64_t target_peprocess = 0;
	((uint64_t(*)(uint64_t, uint64_t*))PsLookupProcessByProcessId)(pid, &target_peprocess);

	uint64_t dtb = 0;
	memory::copy((uint64_t*)(uint8_t*)(target_peprocess + 0x28 /* ->DirectoryTableBase */), &dtb, sizeof(uint64_t));

	return dtb;
}

uint64_t read_phys_mmu(uint64_t address)
{
	uint64_t buffer = 0;
	uint64_t phys_addr = address;
	uint64_t bytes_ret = 0;

	/* we can't use MmMapIoSpaceEx because after windows patch it can't read MMU layers */
	uint64_t MmCopyMemory = memory::get_export_address(global::ntoskrnl, "MmCopyMemory"); 
	((uint64_t(*)(uint64_t*, uint64_t, uint64_t, uint32_t, uint64_t*))MmCopyMemory)(&buffer, phys_addr, sizeof(buffer), 0x1, &bytes_ret);

	return buffer;
}

uint64_t ReadPhysicalMemory(uint64_t address, uint64_t* buffer, uint64_t size)
{
	uint64_t MmMapIoSpaceEx = memory::get_export_address(global::ntoskrnl, "MmMapIoSpaceEx");
	uint64_t MmUnmapIoSpace = memory::get_export_address(global::ntoskrnl, "MmUnmapIoSpace");

	uint64_t mapped_address = ((uint64_t(*)(uint64_t, uint64_t, uint32_t))MmMapIoSpaceEx)(address, size, 0x04);
	if (!mapped_address)
	{
		return 1;
	}

	memory::copy((uint64_t*)mapped_address, buffer, size);

	((void(*)(uint64_t, uint64_t))MmUnmapIoSpace)(mapped_address, size);

	return 0;
}

uint64_t WritePhysicalMemory(uint64_t address, uint64_t* buffer, uint64_t size)
{
	uint64_t MmMapIoSpaceEx = memory::get_export_address(global::ntoskrnl, "MmMapIoSpaceEx");
	uint64_t MmUnmapIoSpace = memory::get_export_address(global::ntoskrnl, "MmUnmapIoSpace");

	uint64_t mapped_address = ((uint64_t(*)(uint64_t, uint64_t, uint32_t))MmMapIoSpaceEx)(address, size, 0x04);
	if (!mapped_address)
	{
		return 1;
	}

	memory::copy(buffer, (uint64_t*)mapped_address, size);

	((void(*)(uint64_t, uint64_t))MmUnmapIoSpace)(mapped_address, size);

	return 0;
}

uint64_t va_to_pa(uint64_t va, uint64_t pid)
{
	/* DTB holds physical address of PML4 structure */
	uint64_t dtb = get_process_dtb(pid);
	dtb &= ~0xf;

	uint64_t pml4_index = (va >> 39) & 0x1FF;
	uint64_t pml4_entry = read_phys_mmu(dtb + (pml4_index * sizeof(uint64_t)));
	if (!(pml4_entry & 1)) /* checking the present bit */
	{
		return 0;
	}

	uint64_t pdpt_index = (va >> 30) & 0x1FF;
	uint64_t pdpt_base = pml4_entry & 0x000FFFFFFFFFF000;
	uint64_t pdpt_entry = read_phys_mmu(pdpt_base + (pdpt_index * sizeof(uint64_t)));
	if (!(pdpt_entry & 1))
	{
		return 0;
	}

	/* check for 1GB large page */
	if (pdpt_entry & 0x80)
	{
		return (pdpt_entry & (~0ull << 42 >> 12)) + (va & ~(~0ull << 30));
	}

	uint64_t pd_index = (va >> 21) & 0x1FF;
	uint64_t pd_base = pdpt_entry & 0x000FFFFFFFFFF000;
	uint64_t pd_entry = read_phys_mmu(pd_base + (pd_index * sizeof(uint64_t)));
	if (!(pd_entry & 1))
	{
		return 0;
	}

	/* check for 2MB large page */
	if (pd_entry & 0x80)
	{
		return (pd_entry & (~0ull << 42 >> 12)) + (va & ~(~0ull << 21));
	}

	uint64_t pt_base = pd_entry & 0x000FFFFFFFFFF000;
	uint64_t pt_index = (va >> 12) & 0x1FF;
	uint64_t pt_entry = read_phys_mmu(pt_base + (pt_index * sizeof(uint64_t)));
	if (!(pt_entry & 1))
	{
		return 0;
	}

	uint64_t page_offset = va & 0xFFF;
	uint64_t phys_base = pt_entry & 0x000FFFFFFFFFF000;
	uint64_t pa = phys_base + page_offset;

	return pa;
}