#pragma once
#include "pch.h"

EFI_STATUS EFIAPI OslArchTransferToKernelHook(uint64_t loader_block_addr, uint64_t entry);