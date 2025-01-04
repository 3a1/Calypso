#pragma once
#include "pch.h"

#define command_magic 0xDEAD

enum class command_type
{
    CopyKernelMemory,
    ReadProcessMemory,
    WriteProcessMemory,
    KillProcess,
    PrivilegeEscalation
};

struct command_t
{
    uint16_t magic;
    command_type type;
    uint64_t data[10];
};