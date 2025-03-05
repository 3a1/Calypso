#include "OslArchTransferToKernel.h"

EFI_STATUS EFIAPI OslArchTransferToKernelHook(uint64_t loader_block_addr, uint64_t entry)
{
    trampoline::Unhook(global::OslArchTransferToKernel, (uint8_t*) &global::OslArchTransferToKernelData);

    global::ntoskrnl = utils::get_module_base((uint64_t)(uint8_t*)(loader_block_addr + 0x10 /* ->LoadOrderListHead */), L"ntoskrnl.exe");
    if (!global::ntoskrnl)
    {
        INFINITY_LOOP();
    }
    Log("ntoskrnl -> %p", global::ntoskrnl);

    global::NtUnloadKey = memory::scan_section(global::ntoskrnl, "PAGE", (uint8_t*)&NtUnloadKeyPattern, sizeof(NtUnloadKeyPattern));
    if (!global::NtUnloadKey)
    {
        global::NtUnloadKey = memory::scan_section(global::ntoskrnl, "PAGE", (uint8_t*)&NtUnloadKeyPattern2, sizeof(NtUnloadKeyPattern2));
        if (!global::NtUnloadKey)
        {
            Error("Can't find NtUnloadKey address!");
        }
    }
    Log("NtUnloadKey -> %p", global::NtUnloadKey);
    
    global::CmUnloadKey = memory::scan_section_ex(global::ntoskrnl, "PAGE", (uint8_t*)&CmUnloadKeyPattern, sizeof(CmUnloadKeyPattern), (uint8_t*)&CmUnloadKeyMask);
    if (!global::CmUnloadKey)
    {
        global::CmUnloadKey = memory::scan_section_ex(global::ntoskrnl, "PAGE", (uint8_t*)&CmUnloadKeyPattern2, sizeof(CmUnloadKeyPattern2), (uint8_t*)&CmUnloadKeyMask2);
        if (!global::CmUnloadKey)
        {
            Error("Can't find CmUnloadKey address!");
        }
    }
    Log("CmUnloadKey -> %p", global::CmUnloadKey);

    trampoline::Hook(global::NtUnloadKey, global::NtUnloadKeyHookAddress, 0);

    return ((EFI_STATUS(*)(uint64_t, uint64_t))global::OslArchTransferToKernel)(loader_block_addr, entry);
}