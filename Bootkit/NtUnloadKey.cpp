#include "NtUnloadKey.h"

uint64_t NtUnloadKeyHook(uint64_t a1)
{
    command_t cmd = *(command_t*)a1;

    if (cmd.magic == command_magic)
    {
        return dispatcher::Start(cmd);
    }

    return ((uint64_t(*)(uint64_t, uint32_t, uint8_t, uint64_t))global::CmUnloadKey)(a1, 0LL, 0LL, 0LL);
}
