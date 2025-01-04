#pragma once
#include "pch.h"

#define TRAMPOLINE_SIZE 12

namespace global
{
    EXTERN_C
    {
        inline EFI_SYSTEM_TABLE*       SystemTable;
        inline EFI_RUNTIME_SERVICES*   RuntimeServices;
        inline EFI_BOOT_SERVICES*      BootServices;

        inline EFI_EVENT               NotifySetVirtualAddressMapEvent;
        inline EFI_EXIT_BOOT_SERVICES  ExitBootServices;
        inline uint64_t                RetExitBootServices;

        inline uint64_t                winload;
        inline uint64_t                ntoskrnl;

        inline uint8_t                 OslArchTransferToKernelData[TRAMPOLINE_SIZE];
        inline uint64_t                OslArchTransferToKernel;

        inline uint64_t                NtUnloadKey;
        inline uint64_t                NtUnloadKeyHookAddress;
        inline uint64_t                CmUnloadKey;
    }
}