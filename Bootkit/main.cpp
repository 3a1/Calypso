#include "pch.h"

VOID EFIAPI NotifySetVirtualAddressMap(EFI_EVENT Event, VOID* Context)
{
    global::NtUnloadKeyHookAddress = (uint64_t)NtUnloadKeyHook;
    global::RuntimeServices->ConvertPointer(0, (void**)&global::NtUnloadKeyHookAddress);
}

EXTERN_C EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    global::RuntimeServices = SystemTable->RuntimeServices;
    global::BootServices    = SystemTable->BootServices;
    global::SystemTable     = SystemTable;

    global::ExitBootServices = global::BootServices->ExitBootServices;
    global::BootServices->ExitBootServices = ExitBootServicesWrapper;

    global::BootServices->CreateEvent(EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE, TPL_NOTIFY, NotifySetVirtualAddressMap, NULL, &global::NotifySetVirtualAddressMapEvent);

    return EFI_SUCCESS;
}