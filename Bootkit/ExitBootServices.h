#pragma once
#include "pch.h"

EXTERN_C EFI_STATUS EFIAPI ExitBootServicesWrapper(IN EFI_HANDLE ImageHandle, IN UINTN MapKey);
EXTERN_C EFI_STATUS EFIAPI ExitBootServicesHook(IN EFI_HANDLE ImageHandle, IN UINTN MapKey);