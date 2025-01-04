#pragma once

#define EXTERN_C extern "C"

EXTERN_C
{
	#include <Uefi.h>
	#include <Library/UefiLib.h>
	#include <Library/DebugLib.h>
	#include <Library/MemoryAllocationLib.h>

	#include <Library/UefiBootServicesTableLib.h>
	#include <Library/UefiRuntimeServicesTableLib.h>

	#include <Uefi/UefiBaseType.h>
	#include <Protocol/LoadedImage.h>
	#include <Library/PrintLib.h>

    void __disable_wp();
    void __enable_wp();
}

#include "struct.h"
#include "general.h"
#include "utils.h"
#include "trampoline.h"
#include "memory.h"
#include "debug.h"
#include "phys.h"
#include "dispatcher.h"

#include "NtUnloadKey.h"
#include "OslArchTransferToKernel.h"
#include "ExitBootServices.h"



//       |\      _,,,---,,_
// Zzz   /,`.-'`'    -.  ;-;;,_
//      |,4-  ) )-,_. ,\ (  `'-'
//     '---''(_/--'  `-\_)