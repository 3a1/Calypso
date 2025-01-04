#include "OslArchTransferToKernel.h"

EFI_STATUS EFIAPI OslArchTransferToKernelHook(uint64_t loader_block_addr, uint64_t entry)
{
    trampoline::Unhook(global::OslArchTransferToKernel, (uint8_t*) &global::OslArchTransferToKernelData);

    global::ntoskrnl = utils::get_module_base(loader_block_addr, L"ntoskrnl.exe");
    if (!global::ntoskrnl)
    {
        INFINITY_LOOP();
    }
    Log("ntoskrnl -> %p", global::ntoskrnl);

    global::NtUnloadKey = memory::scan_section_ex(global::ntoskrnl, "PAGE", (uint8_t*)&NtUnloadKeyPattern, sizeof(NtUnloadKeyPattern), (uint8_t*)&NtUnloadKeyMask);
    if (!global::NtUnloadKey)
    {
        Error("Can't find NtUnloadKey address!");
    }
    Log("NtUnloadKey -> %p", global::NtUnloadKey);
    
    global::CmUnloadKey = memory::scan_section_ex(global::ntoskrnl, "PAGE", (uint8_t*)&CmUnloadKeyPattern, sizeof(CmUnloadKeyPattern), (uint8_t*)&CmUnloadKeyMask);
    if (!global::CmUnloadKey)
    {
        Error("Can't find CmUnloadKey address!");
    }
    Log("CmUnloadKey -> %p", global::CmUnloadKey);

    trampoline::Hook(global::NtUnloadKey, global::NtUnloadKeyHookAddress, 0);

    return ((EFI_STATUS(*)(uint64_t, uint64_t))global::OslArchTransferToKernel)(loader_block_addr, entry);
}