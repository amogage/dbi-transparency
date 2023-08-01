{% raw %}
/*
ARCH: Windows x86
This example was build based on wowGrail and exploits the WOW GATEWAY mechanism to execute x64 bit APIs inside an x86 process
No DBI is capable to pass this test as it would mean to use both 32 and 64 bit DBI libraries
Credits to : 
https://github.com/aaaddress1/wowGrail/blob/master/wowGrail/wowGrail.cpp
https://www.blackhat.com/docs/asia-16/materials/asia-16-Sun-Break-Out-Of-The-Truman-Show-Active-Detection-And-Escape-Of-Dynamic-Binary-Instrumentation.pdf
https://www.youtube.com/watch?v=VGmvx2B5qdo
*/

#include <iostream>
#include <intrin.h>
#include <Windows.h>
using namespace std;
#pragma warning(disable:4996)

#define TEST_FNAME  L"testfile.txt"
#define TEST_BUFFER "hello DBI!\n\n"

#ifndef STATUS_SUCCESS
#   define STATUS_SUCCESS 0
#endif

#pragma pack(push)
#pragma pack(1)
template <class T>
struct _LIST_ENTRY_T
{
    T Flink;
    T Blink;
};

template <class T>
struct _UNICODE_STRING_T
{
    union
    {
        struct
        {
            WORD Length;
            WORD MaximumLength;
        };
        T dummy;
    };
    T Buffer;
};

template <class T>
struct _NT_TIB_T
{
    T ExceptionList;
    T StackBase;
    T StackLimit;
    T SubSystemTib;
    T FiberData;
    T ArbitraryUserPointer;
    T Self;
};

template <class T>
struct _CLIENT_ID
{
    T UniqueProcess;
    T UniqueThread;
};

template <class T>
struct _TEB_T_
{
    _NT_TIB_T<T> NtTib;
    T EnvironmentPointer;
    _CLIENT_ID<T> ClientId;
    T ActiveRpcHandle;
    T ThreadLocalStoragePointer;
    T ProcessEnvironmentBlock;
    DWORD LastErrorValue;
    DWORD CountOfOwnedCriticalSections;
    T CsrClientThread;
    T Win32ThreadInfo;
    DWORD User32Reserved[26];
    //rest of the structure is not defined for now, as it is not needed
};

template <class T>
struct _LDR_DATA_TABLE_ENTRY_T
{
    _LIST_ENTRY_T<T> InLoadOrderLinks;
    _LIST_ENTRY_T<T> InMemoryOrderLinks;
    _LIST_ENTRY_T<T> InInitializationOrderLinks;
    T DllBase;
    T EntryPoint;
    union
    {
        DWORD SizeOfImage;
        T dummy01;
    };
    _UNICODE_STRING_T<T> FullDllName;
    _UNICODE_STRING_T<T> BaseDllName;
    DWORD Flags;
    WORD LoadCount;
    WORD TlsIndex;
    union
    {
        _LIST_ENTRY_T<T> HashLinks;
        struct
        {
            T SectionPointer;
            T CheckSum;
        };
    };
    union
    {
        T LoadedImports;
        DWORD TimeDateStamp;
    };
    T EntryPointActivationContext;
    T PatchInformation;
    _LIST_ENTRY_T<T> ForwarderLinks;
    _LIST_ENTRY_T<T> ServiceTagLinks;
    _LIST_ENTRY_T<T> StaticLinks;
    T ContextInformation;
    T OriginalBase;
    _LARGE_INTEGER LoadTime;
};

template <class T>
struct _PEB_LDR_DATA_T
{
    DWORD Length;
    DWORD Initialized;
    T SsHandle;
    _LIST_ENTRY_T<T> InLoadOrderModuleList;
    _LIST_ENTRY_T<T> InMemoryOrderModuleList;
    _LIST_ENTRY_T<T> InInitializationOrderModuleList;
    T EntryInProgress;
    DWORD ShutdownInProgress;
    T ShutdownThreadId;

};

template <class T, class NGF, int A>
struct _PEB_T
{
    union
    {
        struct
        {
            BYTE InheritedAddressSpace;
            BYTE ReadImageFileExecOptions;
            BYTE BeingDebugged;
            BYTE BitField;
        };
        T dummy01;
    };
    T Mutant;
    T ImageBaseAddress;
    T Ldr;
    T ProcessParameters;
    T SubSystemData;
    T ProcessHeap;
    T FastPebLock;
    T AtlThunkSListPtr;
    T IFEOKey;
    T CrossProcessFlags;
    T UserSharedInfoPtr;
    DWORD SystemReserved;
    DWORD AtlThunkSListPtr32;
    T ApiSetMap;
    T TlsExpansionCounter;
    T TlsBitmap;
    DWORD TlsBitmapBits[2];
    T ReadOnlySharedMemoryBase;
    T HotpatchInformation;
    T ReadOnlyStaticServerData;
    T AnsiCodePageData;
    T OemCodePageData;
    T UnicodeCaseTableData;
    DWORD NumberOfProcessors;
    union
    {
        DWORD NtGlobalFlag;
        NGF dummy02;
    };
    LARGE_INTEGER CriticalSectionTimeout;
    T HeapSegmentReserve;
    T HeapSegmentCommit;
    T HeapDeCommitTotalFreeThreshold;
    T HeapDeCommitFreeBlockThreshold;
    DWORD NumberOfHeaps;
    DWORD MaximumNumberOfHeaps;
    T ProcessHeaps;
    T GdiSharedHandleTable;
    T ProcessStarterHelper;
    T GdiDCAttributeList;
    T LoaderLock;
    DWORD OSMajorVersion;
    DWORD OSMinorVersion;
    WORD OSBuildNumber;
    WORD OSCSDVersion;
    DWORD OSPlatformId;
    DWORD ImageSubsystem;
    DWORD ImageSubsystemMajorVersion;
    T ImageSubsystemMinorVersion;
    T ActiveProcessAffinityMask;
    T GdiHandleBuffer[A];
    T PostProcessInitRoutine;
    T TlsExpansionBitmap;
    DWORD TlsExpansionBitmapBits[32];
    T SessionId;
    ULARGE_INTEGER AppCompatFlags;
    ULARGE_INTEGER AppCompatFlagsUser;
    T pShimData;
    T AppCompatInfo;
    _UNICODE_STRING_T<T> CSDVersion;
    T ActivationContextData;
    T ProcessAssemblyStorageMap;
    T SystemDefaultActivationContextData;
    T SystemAssemblyStorageMap;
    T MinimumStackCommit;
    T FlsCallback;
    _LIST_ENTRY_T<T> FlsListHead;
    T FlsBitmap;
    DWORD FlsBitmapBits[4];
    T FlsHighIndex;
    T WerRegistrationData;
    T WerShipAssertPtr;
    T pContextData;
    T pImageHeaderHash;
    T TracingFlags;
};

typedef _LDR_DATA_TABLE_ENTRY_T<DWORD> LDR_DATA_TABLE_ENTRY32;
typedef _LDR_DATA_TABLE_ENTRY_T<DWORD64> LDR_DATA_TABLE_ENTRY64;

typedef _TEB_T_<DWORD> TEB32;
typedef _TEB_T_<DWORD64> TEB64;

typedef _PEB_LDR_DATA_T<DWORD> PEB_LDR_DATA32;
typedef _PEB_LDR_DATA_T<DWORD64> PEB_LDR_DATA64;

typedef _PEB_T<DWORD, DWORD64, 34> PEB32;
typedef _PEB_T<DWORD64, DWORD, 30> PEB64;

struct _XSAVE_FORMAT64
{
    WORD ControlWord;
    WORD StatusWord;
    BYTE TagWord;
    BYTE Reserved1;
    WORD ErrorOpcode;
    DWORD ErrorOffset;
    WORD ErrorSelector;
    WORD Reserved2;
    DWORD DataOffset;
    WORD DataSelector;
    WORD Reserved3;
    DWORD MxCsr;
    DWORD MxCsr_Mask;
    _M128A FloatRegisters[8];
    _M128A XmmRegisters[16];
    BYTE Reserved4[96];
};

struct _CONTEXT64
{
    DWORD64 P1Home;
    DWORD64 P2Home;
    DWORD64 P3Home;
    DWORD64 P4Home;
    DWORD64 P5Home;
    DWORD64 P6Home;
    DWORD ContextFlags;
    DWORD MxCsr;
    WORD SegCs;
    WORD SegDs;
    WORD SegEs;
    WORD SegFs;
    WORD SegGs;
    WORD SegSs;
    DWORD EFlags;
    DWORD64 Dr0;
    DWORD64 Dr1;
    DWORD64 Dr2;
    DWORD64 Dr3;
    DWORD64 Dr6;
    DWORD64 Dr7;
    DWORD64 Rax;
    DWORD64 Rcx;
    DWORD64 Rdx;
    DWORD64 Rbx;
    DWORD64 Rsp;
    DWORD64 Rbp;
    DWORD64 Rsi;
    DWORD64 Rdi;
    DWORD64 R8;
    DWORD64 R9;
    DWORD64 R10;
    DWORD64 R11;
    DWORD64 R12;
    DWORD64 R13;
    DWORD64 R14;
    DWORD64 R15;
    DWORD64 Rip;
    _XSAVE_FORMAT64 FltSave;
    _M128A Header[2];
    _M128A Legacy[8];
    _M128A Xmm0;
    _M128A Xmm1;
    _M128A Xmm2;
    _M128A Xmm3;
    _M128A Xmm4;
    _M128A Xmm5;
    _M128A Xmm6;
    _M128A Xmm7;
    _M128A Xmm8;
    _M128A Xmm9;
    _M128A Xmm10;
    _M128A Xmm11;
    _M128A Xmm12;
    _M128A Xmm13;
    _M128A Xmm14;
    _M128A Xmm15;
    _M128A VectorRegister[26];
    DWORD64 VectorControl;
    DWORD64 DebugControl;
    DWORD64 LastBranchToRip;
    DWORD64 LastBranchFromRip;
    DWORD64 LastExceptionToRip;
    DWORD64 LastExceptionFromRip;
};

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
}UNICODE_STRING, * PUNICODE_STRING;

typedef struct _IO_STATUS_BLOCK {
#pragma warning(push)
#pragma warning(disable: 4201) // we'll always use the Microsoft compiler
    union {
        NTSTATUS Status;
        PVOID Pointer;
    } DUMMYUNIONNAME;
#pragma warning(pop)

    ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES* POBJECT_ATTRIBUTES;

// Below defines for .ContextFlags field are taken from WinNT.h
#ifndef CONTEXT_AMD64
#define CONTEXT_AMD64 0x100000
#endif

#define CONTEXT64_CONTROL (CONTEXT_AMD64 | 0x1L)
#define CONTEXT64_INTEGER (CONTEXT_AMD64 | 0x2L)
#define CONTEXT64_SEGMENTS (CONTEXT_AMD64 | 0x4L)
#define CONTEXT64_FLOATING_POINT  (CONTEXT_AMD64 | 0x8L)
#define CONTEXT64_DEBUG_REGISTERS (CONTEXT_AMD64 | 0x10L)
#define CONTEXT64_FULL (CONTEXT64_CONTROL | CONTEXT64_INTEGER | CONTEXT64_FLOATING_POINT)
#define CONTEXT64_ALL (CONTEXT64_CONTROL | CONTEXT64_INTEGER | CONTEXT64_SEGMENTS | CONTEXT64_FLOATING_POINT | CONTEXT64_DEBUG_REGISTERS)
#define CONTEXT64_XSTATE (CONTEXT_AMD64 | 0x20L)

#define OBJ_INHERIT                         0x00000002L
#define OBJ_PERMANENT                       0x00000010L
#define OBJ_EXCLUSIVE                       0x00000020L
#define OBJ_CASE_INSENSITIVE                0x00000040L
#define OBJ_OPENIF                          0x00000080L
#define OBJ_OPENLINK                        0x00000100L
#define OBJ_KERNEL_HANDLE                   0x00000200L
#define OBJ_FORCE_ACCESS_CHECK              0x00000400L
#define OBJ_IGNORE_IMPERSONATED_DEVICEMAP   0x00000800L
#define OBJ_DONT_REPARSE                    0x00001000L
#define OBJ_VALID_ATTRIBUTES                0x00001FF2L

#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005
#define FILE_MAXIMUM_DISPOSITION        0x00000005

#define FILE_SYNCHRONOUS_IO_ALERT               0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT            0x00000020
#define FILE_NON_DIRECTORY_FILE                 0x00000040
#define FILE_CREATE_TREE_CONNECTION             0x00000080

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

#pragma pack(pop)

bool flag = false;

typedef void (cdecl* MEMCPY64)(ULONG64, ULONG64, ULONG64);
MEMCPY64 memcpy64;

/*auto memcpy64 = ((void(cdecl*)(ULONG64, ULONG64, ULONG64))((PCSTR)
    // enter 64 bit mode
    "\x6a\x33\xe8\x00\x00\x00\x00\x83\x04\x24\x05\xcb"
    // memcpy for 64 bit
    "\x67\x48\x8b\x7c\x24\x04\x67\x48\x8b\x74\x24\x0c\x67\x48\x8b\x4c\x24\x14\xf3\xa4"
    // exit 64 bit mode
    "\xe8\x00\x00\x00\x00\xc7\x44\x24\x04\x23\x00\x00\x00\x83\x04\x24\x0d\xcb\xc3"
    ));*/

BYTE memcpy64_src[] = {
    //x86
    0x6A,0x33,                  //push 0x33
    0xE8,0x00,0x00,0x00,0x00,   //call $+5
    0x83,0x04,0x24,0x05,        //add [esp], 5
    0xCB,                       //retf
    //x64
    0x67,0x48,0x8B,0x7C,0x24,0x04,				//mov rdi, [esp+0x04]
    0x67,0x48,0x8B,0x74,0x24,0x0C,				//mov rsi, [esp+0x0c]
    0x67,0x48,0x8B,0x4C,0x24,0x14,				//mov rcx, [esp+0x014]
    0xF3,0xA4,									//rep movsb
    0xE8,0x00,0x00,0x00,0x00,					//call $+5
    0xC7,0x44,0x24,0x04,0x23,0x00,0x00,0x00,	//mov dword [rsp+4], 0x23
    0x83,0x04,0x24,0x0D,						//add dword [rsp], 0x0d
    0xCB,										//retf
    0xC3										//ret
};

PEB64* getPtr_Peb64() {
    // mov eax,gs:[00000060]; ret
    BYTE payload[] = { 0x65, 0xA1, 0x60, 0x00, 0x00, 0x00, 0xC3 };
    BYTE* reloc = (BYTE*)VirtualAlloc(0, USN_PAGE_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(reloc, payload, sizeof(payload));
    PEB64* result = ((PEB64 * (cdecl*)())reloc)();
    VirtualFree(reloc, 0, MEM_RELEASE);
    return result;
}


string get64b_CSTR(ULONG64 ptr64bStr) {
    CHAR szBuf[MAX_PATH];
    memcpy64((ULONG64)&szBuf, ptr64bStr, sizeof(szBuf));
    return *new string(szBuf);
}

wstring get64b_WSTR(ULONG64 ptr64bStr) {
    WCHAR szBuf[MAX_PATH];
    memcpy64((ULONG64)&szBuf, ptr64bStr, sizeof(szBuf));
    return *new wstring(szBuf);
}


UINT64 getPtr_Module64(const wchar_t* szDllName) {
    PEB_LDR_DATA64 ldrNode = {};
    LDR_DATA_TABLE_ENTRY64 currNode = {};

    // fetch ldr head node
    memcpy64((ULONG64)&ldrNode, (ULONG64)getPtr_Peb64()->Ldr, sizeof(ldrNode));

    // read the first ldr node (should be the current EXE module)
    for (ULONG64 ptrCurr = ldrNode.InLoadOrderModuleList.Flink;; ptrCurr = currNode.InLoadOrderLinks.Flink) {
        memcpy64((ULONG64)&currNode, ptrCurr, sizeof(currNode));
        if (wcsstr(szDllName, get64b_WSTR(currNode.BaseDllName.Buffer).c_str()))
            return currNode.DllBase;
    }
    return 0;
}

void getPtr_Wow64SystemServiceEx(UINT64& value) {
    auto ptr_wow64Mod = getPtr_Module64(L"wow64.dll");
    printf("[v] current wow64.dll @ %llx\n", ptr_wow64Mod);

    char exeBuf[4096];
    memcpy64((ULONG)&exeBuf, ptr_wow64Mod, sizeof(exeBuf));
    auto k = PIMAGE_NT_HEADERS64(&exeBuf[0] + PIMAGE_DOS_HEADER(exeBuf)->e_lfanew);
    auto rvaExportTable = k->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    memcpy64((ULONG)&exeBuf, ptr_wow64Mod + rvaExportTable, sizeof(exeBuf));

    auto numOfNames = PIMAGE_EXPORT_DIRECTORY(exeBuf)->NumberOfNames;
    auto arrOfNames = new UINT32[numOfNames + 1], arrOfFuncs = new UINT32[numOfNames + 1];
    auto addrOfNameOrds = new UINT16[numOfNames + 1];
    memcpy64((ULONG)arrOfNames, ptr_wow64Mod + PIMAGE_EXPORT_DIRECTORY(exeBuf)->AddressOfNames, sizeof(UINT32) * numOfNames);
    memcpy64((ULONG)addrOfNameOrds, ptr_wow64Mod + PIMAGE_EXPORT_DIRECTORY(exeBuf)->AddressOfNameOrdinals, sizeof(UINT16) * numOfNames);
    memcpy64((ULONG)arrOfFuncs, ptr_wow64Mod + PIMAGE_EXPORT_DIRECTORY(exeBuf)->AddressOfFunctions, sizeof(UINT32) * numOfNames);

    for (size_t i = 0; i < numOfNames; i++) {
        auto currApiName = get64b_CSTR(ptr_wow64Mod + arrOfNames[i]);
        printf("[v] found export API -- %s\n", currApiName.c_str());
        if (strstr("Wow64SystemServiceEx", currApiName.c_str()))
            value = ptr_wow64Mod + arrOfFuncs[addrOfNameOrds[i]];
    }

}

size_t getBytecodeOfNtAPI(const char* ntAPItoLookup) {
    static BYTE* dumpImage = 0;
    if (dumpImage == nullptr) {
        // read whole PE static binary.
        FILE* fileptr; BYTE* buffer; LONGLONG filelen;
        fileptr = fopen("C:/Windows/SysWoW64/ntdll.dll", "rb");
        fseek(fileptr, 0, SEEK_END);
        filelen = ftell(fileptr);
        rewind(fileptr);
        buffer = (BYTE*)malloc((filelen + 1) * sizeof(char));
        fread(buffer, filelen, 1, fileptr);

        // dump static PE binary into image.
        PIMAGE_NT_HEADERS ntHdr = (IMAGE_NT_HEADERS*)(buffer + ((IMAGE_DOS_HEADER*)buffer)->e_lfanew);
        dumpImage = (BYTE*)malloc(ntHdr->OptionalHeader.SizeOfImage);
        memcpy(dumpImage, buffer, ntHdr->OptionalHeader.SizeOfHeaders);
        for (size_t i = 0; i < ntHdr->FileHeader.NumberOfSections; i++) {
            auto curr = PIMAGE_SECTION_HEADER(size_t(ntHdr) + sizeof(IMAGE_NT_HEADERS))[i];
            memcpy(dumpImage + curr.VirtualAddress, buffer + curr.PointerToRawData, curr.SizeOfRawData);
        }
        free(buffer);
        fclose(fileptr);
    }

    // EAT parse.
    PIMAGE_NT_HEADERS ntHdr = (IMAGE_NT_HEADERS*)(dumpImage + ((IMAGE_DOS_HEADER*)dumpImage)->e_lfanew);
    auto a = ntHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    PIMAGE_EXPORT_DIRECTORY ied = (PIMAGE_EXPORT_DIRECTORY)((LPBYTE)dumpImage + a.VirtualAddress);
    uint32_t* addrOfNames = (uint32_t*)((size_t)dumpImage + ied->AddressOfNames);
    uint16_t* addrOfNameOrds = (uint16_t*)((size_t)dumpImage + ied->AddressOfNameOrdinals);
    uint32_t* AddrOfFuncAddrs = (uint32_t*)((size_t)dumpImage + ied->AddressOfFunctions);
    if (ied->NumberOfNames == 0) return (size_t)0;
    for (DWORD i = 0; i < ied->NumberOfNames; i++)
        if (!stricmp((char*)((size_t)dumpImage + addrOfNames[i]), ntAPItoLookup))
            return ((size_t)dumpImage + AddrOfFuncAddrs[addrOfNameOrds[i]]);
    return 0;
}

int NtAPI(const char* szNtApiToCall, ...) {

    PCHAR jit_stub;
    PCHAR apiAddr = PCHAR(getBytecodeOfNtAPI(szNtApiToCall));
    static uint64_t ptrTranslator(0);
    if (!ptrTranslator)
        getPtr_Wow64SystemServiceEx(ptrTranslator);

    static uint8_t stub_template[] = {
        /* +00 - mov eax, 00000000      */ 0xB8, 0x00, 0x00, 0x00, 0x00,
        /* +05 - mov edx, ds:[esp+0x4]  */ 0x8b, 0x54, 0x24, 0x04,
        /* +09 - mov    ecx,eax         */ 0x89, 0xC1,
        /* +0B - enter 64 bit mode      */ 0x6A, 0x33, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x83, 0x04, 0x24, 0x05, 0xCB,
        /* +17 - xchg r14, rsp */        0x49, 0x87, 0xE6,
        /* +1A - call qword ptr [DEADBEEF] */ 0xFF, 0x14, 0x25, 0xEF, 0xBE, 0xAD, 0xDE,
        /* +21 - xchg r14, rsp */ 0x49, 0x87, 0xE6,
        /* +24 - exit 64 bit mode  */ 0xE8, 0x0, 0x0, 0, 0, 0xC7,0x44, 0x24, 4, 0x23, 0, 0, 0, 0x83, 4, 0x24, 0xD, 0xCB,
        0xc3,
    };

    jit_stub = (PCHAR)VirtualAlloc(0, sizeof(stub_template), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(jit_stub, stub_template, sizeof(stub_template));
    va_list    args;
    va_start(args, szNtApiToCall);
    *((uint32_t*)&jit_stub[0x01]) = *(uint32_t*)&apiAddr[1];
    *((uint32_t*)&jit_stub[0x1d]) = (size_t)&ptrTranslator;
    auto ret = ((NTSTATUS(__cdecl*)(...))jit_stub)(args);
    return ret;
}


int RunPortableExecutable(void* Image)
{
    IMAGE_DOS_HEADER* DOSHeader; // For Nt DOS Header symbols
    IMAGE_NT_HEADERS* NtHeader; // For Nt PE Header objects & symbols
    IMAGE_SECTION_HEADER* SectionHeader;

    PROCESS_INFORMATION PI;
    STARTUPINFOA SI;

    CONTEXT* CTX;

    DWORD* ImageBase; //Base address of the image
    void* pImageBase; // Pointer to the image base

    int count;
    char CurrentFilePath[1024] = "C:\\Windows\\SysWOW64\\calc.exe";

    DOSHeader = PIMAGE_DOS_HEADER(Image); // Initialize Variable
    NtHeader = PIMAGE_NT_HEADERS(DWORD(Image) + DOSHeader->e_lfanew); // Initialize
    if (NtHeader->Signature == IMAGE_NT_SIGNATURE) // Check if image is a PE File.
    {
        ZeroMemory(&PI, sizeof(PI)); // Null the memory
        ZeroMemory(&SI, sizeof(SI)); // Null the memory

        if (CreateProcessA(CurrentFilePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &SI, &PI))
        {
            // Allocate memory for the context.
            CTX = LPCONTEXT(VirtualAlloc(NULL, sizeof(CTX), MEM_COMMIT, PAGE_READWRITE));
            CTX->ContextFlags = CONTEXT_FULL; // Context is allocated

            if (GetThreadContext(PI.hThread, LPCONTEXT(CTX))) //if context is in thread
            {

                pImageBase = VirtualAllocEx(PI.hProcess, LPVOID(NtHeader->OptionalHeader.ImageBase), NtHeader->OptionalHeader.SizeOfImage, 0x3000, PAGE_EXECUTE_READWRITE);

                if (pImageBase == 0) {
                    NtAPI("ZwTerminateProcess", PI.hProcess, 0);
                    return 0;
                }

                // Write the image to the process
                NtAPI("NtWriteVirtualMemory", PI.hProcess, pImageBase, Image, NtHeader->OptionalHeader.SizeOfHeaders, NULL);
                for (count = 0; count < NtHeader->FileHeader.NumberOfSections; count++)
                {
                    SectionHeader = PIMAGE_SECTION_HEADER(DWORD(Image) + DOSHeader->e_lfanew + 248 + (count * 40));
                    NtAPI("NtWriteVirtualMemory", PI.hProcess, LPVOID(DWORD(pImageBase) + SectionHeader->VirtualAddress), LPVOID(DWORD(Image) + SectionHeader->PointerToRawData), SectionHeader->SizeOfRawData, 0);
                }
#ifndef _M_X64
                NtAPI("NtWriteVirtualMemory", PI.hProcess, LPVOID(CTX->Ebx + 8), PVOID(&NtHeader->OptionalHeader.ImageBase), 4, 0);
                // Move address of entry point to the eax register
                CTX->Eax = DWORD(pImageBase) + NtHeader->OptionalHeader.AddressOfEntryPoint;
#endif
                NtAPI("NtSetContextThread", PI.hThread, CTX); // Set the context

                DWORD useless;
                NtAPI("NtResumeThread", PI.hThread, &useless); //Start the process/call main()

                return 0; // Operation was successful.
            }
        }
    }
    return 0;
}

BYTE* MapFileToMemory(const char filename[])
{
    FILE* fileptr = 0;
    BYTE* buffer = 0;

    fileptr = fopen(filename, "rb");        // Open the file in binary mode
    if (!fileptr)
    {
        printf("[hello.exe] not found\n");
        return 0;
    }
    else
    {
        fseek(fileptr, 0, SEEK_END);            // Jump to the end of the file
        long filelen = ftell(fileptr);          // Get the current byte offset in the file
        rewind(fileptr);                        // Jump back to the beginning of the file

        buffer = (BYTE*)malloc((filelen + 1) * sizeof(char)); // Enough memory for file + \0
        fread(buffer, filelen, 1, fileptr);     // Read in the entire file
        fclose(fileptr);                        // Close the file
    }

    return buffer;
}

int main()
{
    memcpy64 = (MEMCPY64)VirtualAlloc(0, USN_PAGE_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(memcpy64, memcpy64_src, sizeof(memcpy64_src));
    //sourcecode for hello.exe:  https://flatassembler.net/fasmw17330.zip --> fasm\examples\hello.asm (build with fasmw.exe ctrl+f9 after opening the file)
    RunPortableExecutable(MapFileToMemory("hello.exe"));
    VirtualFree(memcpy64, 0, MEM_RELEASE);
    printf("PASSED");
    return 0;
}

{% endraw %}