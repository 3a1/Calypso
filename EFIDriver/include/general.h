#define DRIVER_SIZE 2097152 // 2mb should be enough
__attribute__((section(".text"))) char DriverBuffer[DRIVER_SIZE]; // this fixing BSOD on some systems

// Defines used to check if call is really coming from client
#define baseOperation 0x6256
#define VARIABLE_NAME L"XTbdNErLv"

//This is only to modify every command/magic key with only 1 def and don't need to go everywhere, the compiler will automatically parse the operation to number
#define COMMAND_MAGIC baseOperation*0x7346

#define COPY_OPERATION baseOperation * 0x823
#define BASE_OPERATION baseOperation * 0x289
#define EXPORT_OPERATION baseOperation * 0x612

// Dummy protocol struct
typedef struct _DummyProtocalData{
	UINTN blank;
} DummyProtocalData;

typedef unsigned long long ptr64;

// Struct containing data used to communicate with the client
typedef struct _MemoryCommand 
{
	int magic;
	int operation;
	ptr64 data[10];
} MemoryCommand;

// Functions (Windows only)
typedef int (MicrosoftCallingType *PsLookupProcessByProcessId)(
	void* ProcessId,
	void* OutPEProcess
);
typedef void* (MicrosoftCallingType *PsGetProcessSectionBaseAddress)(
	void* PEProcess
);
typedef int (MicrosoftCallingType *MmCopyVirtualMemory)(
	void* SourceProcess,
	void* SourceAddress,
	void* TargetProcess,
	void* TargetAddress,
	ptr64 BufferSize,
	char PreviousMode,
	void* ReturnSize
);

// Our protocol GUID (should be different for every driver)
static const EFI_GUID ProtocolGuid
	= { 0x2f84893e, 0xfd5e, 0x2038, {0x8d, 0x9e, 0x20, 0xa7, 0xaf, 0x9c, 0x32, 0xf1} };

// VirtualAddressMap GUID (gEfiEventVirtualAddressChangeGuid)
static const EFI_GUID VirtualGuid
	= { 0x13FA7698, 0xC831, 0x49C7, { 0x87, 0xEA, 0x8F, 0x43, 0xFC, 0xC2, 0x51, 0x96 }}; //we will remove later shouldn't be important

// ExitBootServices GUID (gEfiEventExitBootServicesGuid)
static const EFI_GUID ExitGuid
	= { 0x27ABF055, 0xB1B8, 0x4C26, { 0x80, 0x48, 0x74, 0x8F, 0x37, 0xBA, 0xA2, 0xDF }}; //we will remove later shouldn't be important

// Pointers to original functions
static EFI_SET_VARIABLE oSetVariable = NULL;

// Global declarations
static EFI_EVENT NotifyEvent = NULL;
static EFI_EVENT ExitEvent = NULL;
static BOOLEAN Virtual = FALSE;
static BOOLEAN Runtime = FALSE;

static PsLookupProcessByProcessId GetProcessByPid = (PsLookupProcessByProcessId)0;
static PsGetProcessSectionBaseAddress GetBaseAddress = (PsGetProcessSectionBaseAddress)0;
static MmCopyVirtualMemory MCopyVirtualMemory = (MmCopyVirtualMemory)0;

void CopyMemory(void* destination, const void* source, uint64_t size) {
    uint8_t* dst = (uint8_t*)destination;
    const uint8_t* src = (const uint8_t*)source;
    for (uint64_t i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

