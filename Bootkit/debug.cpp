#include "debug.h"

DbgPrintFn DbgPrint = nullptr;
bool isInitialized = false;

void InitializeDebug()
{
    if (!isInitialized && global::ntoskrnl != 0)
    {
        uint64_t address = memory::get_export_address(global::ntoskrnl, "DbgPrint");
        DbgPrint = (DbgPrintFn)address;
        isInitialized = true;
    }
}