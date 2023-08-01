{% raw %}
/*
ARCH: Windows x86
This example creates 30k threads, by relocating a piece of code to dynamically allocated payloads
The code does not depends on any library, it dynamically loads all the requirements from PEB
DBIs either try to sync the executable payloads with the DBI-cache, or to continously allocate different cache portions, which exhausts the DBI-cache limits
A native execution usually don't reach over 200MB memory space and goes up to 30k successfully allocations, while PIN and DynamoRIO both crash with much higher memory footprints and lower allocation counts
*/

#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)

#define FILE_SHARE_READ                 0x00000001  
#define FILE_SHARE_WRITE                0x00000002  
#define FILE_SHARE_DELETE               0x00000004  

#define INFINITE -1
#define STD_OUTPUT_HANDLE ((DWORD)-11)

#define FILE_ATTRIBUTE_READONLY             0x00000001  
#define FILE_ATTRIBUTE_HIDDEN               0x00000002  
#define FILE_ATTRIBUTE_SYSTEM               0x00000004  
#define FILE_ATTRIBUTE_DIRECTORY            0x00000010  
#define FILE_ATTRIBUTE_ARCHIVE              0x00000020  
#define FILE_ATTRIBUTE_DEVICE               0x00000040  
#define FILE_ATTRIBUTE_NORMAL               0x00000080  
#define FILE_ATTRIBUTE_TEMPORARY            0x00000100  
#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200  
#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400  
#define FILE_ATTRIBUTE_COMPRESSED           0x00000800  
#define FILE_ATTRIBUTE_OFFLINE              0x00001000  
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000  
#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000  
#define FILE_ATTRIBUTE_VIRTUAL              0x00010000  

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

#define FALSE 0
#define TRUE 1

#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2

#define PAGE_NOACCESS          0x01     
#define PAGE_READONLY          0x02     
#define PAGE_READWRITE         0x04     
#define PAGE_WRITECOPY         0x08     
#define PAGE_EXECUTE           0x10     
#define PAGE_EXECUTE_READ      0x20     
#define PAGE_EXECUTE_READWRITE 0x40     
#define PAGE_EXECUTE_WRITECOPY 0x80     
#define PAGE_GUARD            0x100     
#define PAGE_NOCACHE          0x200     
#define PAGE_WRITECOMBINE     0x400     
#define MEM_COMMIT           0x1000     
#define MEM_RESERVE          0x2000     
#define MEM_DECOMMIT         0x4000     
#define MEM_RELEASE          0x8000     
#define MEM_FREE            0x10000     
#define MEM_PRIVATE         0x20000     
#define MEM_MAPPED          0x40000     
#define MEM_RESET           0x80000     
#define MEM_TOP_DOWN       0x100000     
#define MEM_WRITE_WATCH    0x200000     
#define MEM_PHYSICAL       0x400000     
#define MEM_ROTATE         0x800000     
#define MEM_LARGE_PAGES  0x20000000     
#define MEM_4MB_PAGES    0x80000000     
#define SEC_FILE           0x800000     
#define SEC_IMAGE         0x1000000     
#define SEC_PROTECTED_IMAGE  0x2000000  
#define SEC_RESERVE       0x4000000     
#define SEC_COMMIT        0x8000000     
#define SEC_NOCACHE      0x10000000     
#define SEC_WRITECOMBINE 0x40000000     
#define SEC_LARGE_PAGES  0x80000000     
#define MEM_IMAGE         SEC_IMAGE     
#define WRITE_WATCH_FLAG_RESET 0x01    

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES        16

typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef unsigned int DWORD;
typedef int BOOL;

typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
    WORD   e_magic;                     // Magic number
    WORD   e_cblp;                      // Bytes on last page of file
    WORD   e_cp;                        // Pages in file
    WORD   e_crlc;                      // Relocations
    WORD   e_cparhdr;                   // Size of header in paragraphs
    WORD   e_minalloc;                  // Minimum extra paragraphs needed
    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
    WORD   e_ss;                        // Initial (relative) SS value
    WORD   e_sp;                        // Initial SP value
    WORD   e_csum;                      // Checksum
    WORD   e_ip;                        // Initial IP value
    WORD   e_cs;                        // Initial (relative) CS value
    WORD   e_lfarlc;                    // File address of relocation table
    WORD   e_ovno;                      // Overlay number
    WORD   e_res[4];                    // Reserved words
    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
    WORD   e_oeminfo;                   // OEM information; e_oemid specific
    WORD   e_res2[10];                  // Reserved words
    DWORD   e_lfanew;                    // File address of new exe header
} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
    WORD    Machine;
    WORD    NumberOfSections;
    DWORD   TimeDateStamp;
    DWORD   PointerToSymbolTable;
    DWORD   NumberOfSymbols;
    WORD    SizeOfOptionalHeader;
    WORD    Characteristics;
} IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
    DWORD   VirtualAddress;
    DWORD   Size;
} IMAGE_DATA_DIRECTORY, * PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER {
    WORD    Magic;
    BYTE    MajorLinkerVersion;
    BYTE    MinorLinkerVersion;
    DWORD   SizeOfCode;
    DWORD   SizeOfInitializedData;
    DWORD   SizeOfUninitializedData;
    DWORD   AddressOfEntryPoint;
    DWORD   BaseOfCode;
    DWORD   BaseOfData;
    // NT ADDITIONAL
    DWORD   ImageBase;
    DWORD   SectionAlignment;
    DWORD   FileAlignment;
    WORD    MajorOperatingSystemVersion;
    WORD    MinorOperatingSystemVersion;
    WORD    MajorImageVersion;
    WORD    MinorImageVersion;
    WORD    MajorSubsystemVersion;
    WORD    MinorSubsystemVersion;
    DWORD   Win32VersionValue;
    DWORD   SizeOfImage;
    DWORD   SizeOfHeaders;
    DWORD   CheckSum;
    WORD    Subsystem;
    WORD    DllCharacteristics;
    DWORD   SizeOfStackReserve;
    DWORD   SizeOfStackCommit;
    DWORD   SizeOfHeapReserve;
    DWORD   SizeOfHeapCommit;
    DWORD   LoaderFlags;
    DWORD   NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER, * PIMAGE_OPTIONAL_HEADER;


typedef struct _IMAGE_OPTIONAL_HEADER64 {
    WORD        Magic;
    BYTE        MajorLinkerVersion;
    BYTE        MinorLinkerVersion;
    DWORD       SizeOfCode;
    DWORD       SizeOfInitializedData;
    DWORD       SizeOfUninitializedData;
    DWORD       AddressOfEntryPoint;
    DWORD       BaseOfCode;
    unsigned long long ImageBase;
    DWORD       SectionAlignment;
    DWORD       FileAlignment;
    WORD        MajorOperatingSystemVersion;
    WORD        MinorOperatingSystemVersion;
    WORD        MajorImageVersion;
    WORD        MinorImageVersion;
    WORD        MajorSubsystemVersion;
    WORD        MinorSubsystemVersion;
    DWORD       Win32VersionValue;
    DWORD       SizeOfImage;
    DWORD       SizeOfHeaders;
    DWORD       CheckSum;
    WORD        Subsystem;
    WORD        DllCharacteristics;
    unsigned long long   SizeOfStackReserve;
    unsigned long long   SizeOfStackCommit;
    unsigned long long   SizeOfHeapReserve;
    unsigned long long   SizeOfHeapCommit;
    DWORD       LoaderFlags;
    DWORD       NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, * PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_NT_HEADERS64 {
    DWORD Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, * PIMAGE_NT_HEADERS64;

typedef struct _IMAGE_NT_HEADERS {
    DWORD Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER OptionalHeader;
} IMAGE_NT_HEADERS, * PIMAGE_NT_HEADERS;

typedef struct _IMAGE_EXPORT_DIRECTORY {
    DWORD   Characteristics;
    DWORD   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    DWORD   Name;
    DWORD   Base;
    DWORD   NumberOfFunctions;
    DWORD   NumberOfNames;
    DWORD   AddressOfFunctions;     // RVA from base of image
    DWORD   AddressOfNames;         // RVA from base of image
    DWORD   AddressOfNameOrdinals;  // RVA from base of image
} IMAGE_EXPORT_DIRECTORY, * PIMAGE_EXPORT_DIRECTORY;

//----------------------------------------------------------------------------------------------

struct M_LIST_ENTRY {
    void* flink;
    void* blink;
};

struct M_LDR_DATA_TABLE_ENTRY {
    M_LIST_ENTRY loadOrder;
    M_LIST_ENTRY memOrder;
    M_LIST_ENTRY initOrder;
    void* BaseAddress;
    void* EntryPoint;
    DWORD SizeofImage;
    DWORD reserved1;
#ifdef _M_X64
    WORD reserved2;
    WORD FullLen;
    DWORD reserved3;
    wchar_t* FullDLLName;
    WORD reserved4;
    WORD BaseLen;
    DWORD reserved5;
    wchar_t* BaseDLLName;
#else
    wchar_t* FullDLLName;
    DWORD reserved2;
    wchar_t* BaseDLLName;
#endif
    DWORD Flags;
    WORD LoadCount;
    WORD TlsIndex;
    M_LIST_ENTRY HashTableEntry;
    DWORD TimeDateStamp;
    DWORD reserved6;
    size_t reserved7;
};

struct M_PEB_LDR_DATA {
    DWORD Length;
    BOOL Initialized;
    void* SsHandle;
    M_LIST_ENTRY loadOrder;
    M_LIST_ENTRY memOrder;
    M_LIST_ENTRY initOrder;
};

struct M_USER_PROCESS_PARAMETERS {
    DWORD MaximumLength;
    DWORD Length;
    DWORD Flags;
    DWORD DebugFlags;
    void* ConsoleHandle;
    size_t ConsoleFlags;
    DWORD reserved1;
    void* StdInputHandle;
    void* StdOutputHandle;
    void* StdErrorHandle;
    size_t CurrentDirectoryPath;    //unicode_string
    void* CurrentDirectoryHandle;
    size_t reserved2;
    size_t DllPath;                 //unicode_string
    size_t reserved3;
    size_t ImagePathName;           //unicode_string
    size_t reserved4;
    size_t CommandLine;             //unicode_string
    void* Environment;
    DWORD StartingPositionLeft;
    DWORD StartingPositionTop;
    DWORD Width;
    DWORD Height;
    DWORD CharWidth;
    DWORD CharHeight;
    size_t ConsoleTextAttributes;
    size_t reserved5;
    DWORD WindowFlags;
    DWORD ShowWindowFlags;
    size_t WindowTitle;             //unicode_string
    size_t reserved6;
    size_t DesktopName;             //unicode_string
    size_t ShellInfo;               //unicode_string
    size_t RuntimeData;             //unicode_string
    size_t DLCurrentDirectory;
};

struct M_PEB {
    WORD reserved1;
    BYTE BeingDebugged;
    BYTE reserved2;
#ifdef _M_X64
    DWORD reserved3[5];
#else
    DWORD reserved3[2];
#endif
    M_PEB_LDR_DATA* LoaderData;
    M_USER_PROCESS_PARAMETERS* ProcessParameters;
#ifdef _M_X64
    BYTE reserved4[144];
#else
    BYTE reserved4[104];
#endif
    DWORD NumberOfProcessors;
    BYTE reserved5[92];
    DWORD OSMajVer;
    DWORD OSMinVer;
    WORD BuildNumber;
    WORD reserved6;
    DWORD OSPlatformId;
    DWORD imgSubSys;
    DWORD imgSubSysMajVer;
    DWORD imgSubSysMinVer;
    BYTE reserved7[252];
    void* postInitRoutine;
    BYTE reserved8[136];
    DWORD SessionId;
};

typedef struct _MEMORY_BASIC_INFORMATION {
    void* BaseAddress;
    void* AllocationBase;
    DWORD  AllocationProtect;
    size_t RegionSize;
    DWORD  State;
    DWORD  Protect;
    DWORD  Type;
} MEMORY_BASIC_INFORMATION;

typedef struct _OVERLAPPED {
    size_t Internal;
    size_t InternalHigh;
    union _P {
        struct _S {
            DWORD Offset;
            DWORD OffsetHigh;
        };
        void* Pointer;
    };
    void* hEvent;
} OVERLAPPED;

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;

typedef struct _SECURITY_ATTRIBUTES {
    DWORD  nLength;
    void* lpSecurityDescriptor;
    BOOL   bInheritHandle;
} SECURITY_ATTRIBUTES, * PSECURITY_ATTRIBUTES, * LPSECURITY_ATTRIBUTES;

extern M_PEB* GetPEB32();
extern M_PEB* GetPEB64();

#define LOADLIB_FUNC 0x747A5761
#define GETPADR_FUNC 0x74221D65

typedef DWORD(*THREAD_START_ROUTINE)(void* param);

typedef void* (__stdcall* LOADLIBRARY)(const char* lpFileName);
typedef void* (__stdcall* GETPROCADDRESS)(void* hModule, const char* lpProcName);

typedef void* (__stdcall* VIRTUALALLOC)(void* addr, size_t size, DWORD aloc_type, DWORD protect);
typedef BOOL(__stdcall* VIRTUALFREE)(void* addr, size_t size, DWORD freetype);
typedef size_t(__stdcall* VIRTUALQUERYEX)(void* hProc, void* lpAddr, MEMORY_BASIC_INFORMATION* lpBuffer, size_t length);
typedef void* (__stdcall* CREATEFILE)(char* fname, DWORD access, DWORD share, SECURITY_ATTRIBUTES* lpSecAttrs, DWORD creation, DWORD attrsflags, void* FileTemplate);
typedef BOOL(__stdcall* CLOSEHANDLE)(void* hObject);
typedef BOOL(__stdcall* READFILE)(void* hFile, void* lpBuffer, DWORD size, DWORD* read, OVERLAPPED* lpOverlapped);
typedef BOOL(__stdcall* WRITEFILE)(void* hFile, void* lpBuffer, DWORD size, DWORD* written, OVERLAPPED* lpOverlapped);
typedef DWORD(__stdcall* SETFILEPOINTER)(void* hFile, DWORD distLow, DWORD* distHigh, DWORD method);
typedef DWORD(__stdcall* GETFILESIZE)(void* handle, DWORD* fileSizeHigh);
typedef void* (__stdcall* CREATETHREAD)(SECURITY_ATTRIBUTES* lpSecAttrs, size_t stacksz, THREAD_START_ROUTINE* startaddr, void* param, DWORD creationflags, DWORD* tid);
typedef void(__stdcall* ZEROMEMORY)(void* Dest, size_t length);
typedef void* (__stdcall* CREATEFILEMAPPING)(void* hFile, SECURITY_ATTRIBUTES* lpAttrs, DWORD flProtect, DWORD MaxSzH, DWORD MaxSzL, char* name);
typedef void* (__stdcall* MAPVIEWOFFILE)(void* mapObj, DWORD access, DWORD FileOffsH, DWORD FileOffsL, size_t bytes);
typedef BOOL(__stdcall* UNMAPVIEWOFFILE)(void* BaseAddr);
typedef void(__stdcall* SLEEP)(DWORD milis);
typedef void(__stdcall* GETLOCALTIME)(SYSTEMTIME* lpSysTime);
typedef BOOL(__stdcall* QUERYPERFORMANCECOUNTER)(long long* lpPerformanceCount);
typedef BOOL(__stdcall* QUERYPERFORMANCEFREQUENCY)(long long* lpFrequency);
typedef int(__stdcall* MESSAGEBOX)(DWORD, const char*, const char*, DWORD);
typedef void(__stdcall* WAITFORSINGLEOBJECT)(void*, int);
typedef void(__stdcall* WAITFORMULTIPLEOBJECTS)(DWORD, void**, BOOL, DWORD);
typedef void* (__stdcall* GETSTDHANDLE)(DWORD);
typedef int(*SPRINTF)(char*, const char*, ...);
typedef int(*STRLEN)(const char*);

struct Kernel32Access {
    void* K32Handle;
    void* NTDHandle;
    void* ADVHandle;
    void* USRHandle;
    char appname[256];
    LOADLIBRARY LoadLibraryAddress;
    GETPROCADDRESS GetProcAddress;
};

struct APIInterface {
    VIRTUALALLOC VirtualAlloc;
    VIRTUALFREE VirtualFree;
    VIRTUALQUERYEX VirtualQueryEx;
    CREATEFILE CreateFile;
    CLOSEHANDLE CloseHandle;
    READFILE ReadFile;
    WRITEFILE WriteFile;
    CREATETHREAD CreateThread;
    CREATEFILEMAPPING CrateFileMaping;
    MAPVIEWOFFILE MapViewOfFile;
    UNMAPVIEWOFFILE UnmapViewOfFile;
    SLEEP Sleep;
    GETLOCALTIME GetLocalTime;
    SETFILEPOINTER SetFilePointer;
    GETFILESIZE GetFileSize;
    QUERYPERFORMANCECOUNTER QueryPerformanceCounter;
    QUERYPERFORMANCEFREQUENCY QueryPerformanceFrequency;
    MESSAGEBOX MessageBox;
    WAITFORSINGLEOBJECT WaitForSingleObject;
    WAITFORMULTIPLEOBJECTS WaitForMultipleObjects;
    GETSTDHANDLE GetStdHandle;
    SPRINTF sprintf;
    STRLEN strlen;
};

//--------------------------------------------------------------

void BringMeFullAccess(Kernel32Access& k32h, char strings[30][50]);
void RefreshAPIs(APIInterface& capi, Kernel32Access k32h, char strings[30][50]);

typedef DWORD(__stdcall* PTHREAD_START_ROUTINE)(void* lpThreadParameter);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;
//--------------------------------------------------------------

char strings[30][50] = { 0 };
#define MAX_THREADS 30000

void InitializeStrings(char strings[30][50])
{
    *(DWORD*)strings[0] = 'solC';
    *(DWORD*)(strings[0] + 4) = 'naHe';
    *(DWORD*)(strings[0] + 8) = 'eld';

    *(DWORD*)strings[1] = 'triV';
    *(DWORD*)(strings[1] + 4) = 'Alau';
    *(DWORD*)(strings[1] + 8) = 'coll';

    *(DWORD*)strings[2] = 'triV';
    *(DWORD*)(strings[2] + 4) = 'Flau';
    *(DWORD*)(strings[2] + 8) = 'eer';

    *(DWORD*)strings[3] = 'triV';
    *(DWORD*)(strings[3] + 4) = 'Qlau';
    *(DWORD*)(strings[3] + 8) = 'yreu';
    *(DWORD*)(strings[3] + 12) = 'xE';

    *(DWORD*)strings[4] = 'aerC';
    *(DWORD*)(strings[4] + 4) = 'iFet';
    *(DWORD*)(strings[4] + 8) = 'Ael';

    *(DWORD*)strings[5] = 'daeR';
    *(DWORD*)(strings[5] + 4) = 'eliF';

    *(DWORD*)strings[6] = 'tirW';
    *(DWORD*)(strings[6] + 4) = 'liFe';
    *(DWORD*)(strings[6] + 8) = 'e';

    *(DWORD*)strings[7] = 'aerC';
    *(DWORD*)(strings[7] + 4) = 'hTet';
    *(DWORD*)(strings[7] + 8) = 'daer';

    *(DWORD*)strings[8] = 'aerC';
    *(DWORD*)(strings[8] + 4) = 'iFet';
    *(DWORD*)(strings[8] + 8) = 'aMel';
    *(DWORD*)(strings[8] + 12) = 'nipp';
    *(DWORD*)(strings[8] + 16) = 'Ag';

    *(DWORD*)strings[9] = 'VpaM';
    *(DWORD*)(strings[9] + 4) = 'Owei';
    *(DWORD*)(strings[9] + 8) = 'liFf';
    *(DWORD*)(strings[9] + 12) = 'e';

    *(DWORD*)strings[10] = 'amnU';
    *(DWORD*)(strings[10] + 4) = 'eiVp';
    *(DWORD*)(strings[10] + 8) = 'FfOw';
    *(DWORD*)(strings[10] + 12) = 'eli';

    *(DWORD*)strings[11] = 'eelS';
    *(DWORD*)(strings[11] + 4) = 'p';

    *(DWORD*)strings[12] = 'LteG';
    *(DWORD*)(strings[12] + 4) = 'laco';
    *(DWORD*)(strings[12] + 8) = 'emiT';

    *(DWORD*)strings[13] = 'FteS';
    *(DWORD*)(strings[13] + 4) = 'Peli';
    *(DWORD*)(strings[13] + 8) = 'tnio';
    *(DWORD*)(strings[13] + 12) = 're';

    *(DWORD*)strings[14] = 'FteG';
    *(DWORD*)(strings[14] + 4) = 'Seli';
    *(DWORD*)(strings[14] + 8) = 'ezi';

    *(DWORD*)strings[15] = 'reuQ';
    *(DWORD*)(strings[15] + 4) = 'rePy';
    *(DWORD*)(strings[15] + 8) = 'mrof';
    *(DWORD*)(strings[15] + 12) = 'ecna';
    *(DWORD*)(strings[15] + 16) = 'nuoC';
    *(DWORD*)(strings[15] + 20) = 'ret';

    *(DWORD*)strings[16] = 'reuQ';
    *(DWORD*)(strings[16] + 4) = 'rePy';
    *(DWORD*)(strings[16] + 8) = 'mrof';
    *(DWORD*)(strings[16] + 12) = 'ecna';
    *(DWORD*)(strings[16] + 16) = 'qerF';
    *(DWORD*)(strings[16] + 20) = 'cneu';
    *(DWORD*)(strings[16] + 24) = 'y';

    *(DWORD*)strings[17] = 'sseM';
    *(DWORD*)(strings[17] + 4) = 'Bega';
    *(DWORD*)(strings[17] + 8) = 'Axo';

    *(DWORD*)strings[18] = 'tiaW';
    *(DWORD*)(strings[18] + 4) = 'SroF';
    *(DWORD*)(strings[18] + 8) = 'lgni';
    *(DWORD*)(strings[18] + 12) = 'jbOe';
    *(DWORD*)(strings[18] + 16) = 'tce';

    *(DWORD*)strings[19] = 'tiaW';
    *(DWORD*)(strings[19] + 4) = 'MroF';
    *(DWORD*)(strings[19] + 8) = 'itlu';
    *(DWORD*)(strings[19] + 12) = 'Oelp';
    *(DWORD*)(strings[19] + 16) = 'cejb';
    *(DWORD*)(strings[19] + 20) = 'st';

    //------------------------------------------------
    *(DWORD*)strings[20] = 'lyaP';
    *(DWORD*)(strings[20] + 4) = ' dao';
    *(DWORD*)(strings[20] + 8) = 'nnur';
    *(DWORD*)(strings[20] + 12) = ' gni';
    *(DWORD*)(strings[20] + 16) = 'morf';
    *(DWORD*)(strings[20] + 20) = 'mer ';
    *(DWORD*)(strings[20] + 24) = ' eto';
    *(DWORD*)(strings[20] + 28) = 'corp';
    *(DWORD*)(strings[20] + 32) = '\x00sse';

    *(DWORD*)strings[21] = 'SteG';
    *(DWORD*)(strings[21] + 4) = 'aHdt';
    *(DWORD*)(strings[21] + 8) = 'eldn';

    *(DWORD*)strings[22] = 'irps';
    *(DWORD*)(strings[22] + 4) = 'ftn';

    *(DWORD*)strings[23] = 'lrts';
    *(DWORD*)(strings[23] + 4) = 'ne';

    //------------------------------------------------

    *(DWORD*)strings[26] = 'ldtn';
    *(DWORD*)(strings[26] + 4) = 'ld.l';
    *(DWORD*)(strings[26] + 8) = 'l';

    *(DWORD*)strings[27] = 'avda';
    *(DWORD*)(strings[27] + 4) = '23ip';
    *(DWORD*)(strings[27] + 8) = 'lld.';

    *(DWORD*)strings[28] = 'resu';
    *(DWORD*)(strings[28] + 4) = 'd.23';
    *(DWORD*)(strings[28] + 8) = 'll';

    *(WORD*)(&strings[29][0]) = L'k';
    *(WORD*)(&strings[29][2]) = L'e';
    *(WORD*)(&strings[29][4]) = L'r';
    *(WORD*)(&strings[29][6]) = L'n';
    *(WORD*)(&strings[29][8]) = L'e';
    *(WORD*)(&strings[29][10]) = L'l';
    *(WORD*)(&strings[29][12]) = L'3';
    *(WORD*)(&strings[29][14]) = L'2';
    *(WORD*)(&strings[29][16]) = L'.';
    *(WORD*)(&strings[29][18]) = L'd';
    *(WORD*)(&strings[29][20]) = L'l';
    *(WORD*)(&strings[29][22]) = L'l';
}

void BringMeFullAccess(Kernel32Access& k32h, char strings[30][50])
{
    M_LDR_DATA_TABLE_ENTRY* table_entry_scanner;
#ifdef _M_X64
    IMAGE_NT_HEADERS64* nthdr;
#else
    IMAGE_NT_HEADERS* nthdr;
#endif
    IMAGE_EXPORT_DIRECTORY* imexp;
    size_t AddressOfNames = 0;
    size_t AddressOfNamesVA;
    size_t index = 0;
    char* fname;
    DWORD namehash = 0;
    size_t real_index;
    size_t loadlib_addr = 0;
    size_t getpadd_addr = 0;
    size_t total_found = 0;
    int i;

    //Get PEB address
    M_PEB* peb = 0;
    __asm {
        mov eax, dword ptr fs:[30h]
        mov peb, eax
    }

    table_entry_scanner = (M_LDR_DATA_TABLE_ENTRY*)peb->LoaderData->loadOrder.flink;
    for (i = 0; table_entry_scanner->BaseDLLName[i]; i++)
        k32h.appname[i] = (char)table_entry_scanner->BaseDLLName[i];
    k32h.appname[i] = 0;
    while (table_entry_scanner)
    {
        wchar_t* sl = table_entry_scanner->BaseDLLName;
        wchar_t* sr = (wchar_t*)strings[29];
        int i;
        for (i = 0; sl[i]; i++)
            ;        
        int l = i;
        for (i = 0; sr[i]; i++)
            ;
        int r = i;
        int result = 0;    

        if (l < r)
            result = -1;
        else
            if (l > r)
                result = 1;
            else
            {
                int m = (l <= r) ? l : r;
                for (int i = 0; i < m; i++)
                {
                    wchar_t lc = (wchar_t)(sl[i] | 0x20);
                    wchar_t rc = (wchar_t)(sr[i] | 0x20);
                    if (lc < rc)
                    {
                        result = -1;
                        break;
                    }
                    else
                        if (lc > rc)
                        {
                            result = 1;
                            break;
                        }
                }                
            }        

        if (!result)
        {
            k32h.K32Handle = (void*)table_entry_scanner->BaseAddress;
            break;
        }
        table_entry_scanner = (M_LDR_DATA_TABLE_ENTRY*)table_entry_scanner->loadOrder.flink;
    }

    nthdr = (IMAGE_NT_HEADERS*)(((IMAGE_DOS_HEADER*)k32h.K32Handle)->e_lfanew + (size_t)k32h.K32Handle);
    imexp = (IMAGE_EXPORT_DIRECTORY*)(nthdr->OptionalHeader.DataDirectory[0].VirtualAddress + (size_t)k32h.K32Handle);

    AddressOfNames = imexp->AddressOfNames;
    AddressOfNamesVA = AddressOfNames + (size_t)k32h.K32Handle;

    for (i = 0; i < (int)imexp->NumberOfNames; i++)
    {
        AddressOfNamesVA += 4;
        index++;
        fname = (char*)(*(DWORD*)AddressOfNamesVA + (size_t)k32h.K32Handle);
        namehash = 0;
        for (i = 0; fname[i]; i++)
        {
            namehash ^= fname[i];
            namehash = (namehash << 8) | (namehash >> 24);
        }

        if (namehash == LOADLIB_FUNC)
        {
            real_index = (size_t) * (WORD*)(imexp->AddressOfNameOrdinals + 2 * index + (size_t)k32h.K32Handle);
            loadlib_addr = *(DWORD*)(imexp->AddressOfFunctions + 4 * real_index + (size_t)k32h.K32Handle) + (size_t)k32h.K32Handle;
            total_found++;
        }
        else
            if (namehash == GETPADR_FUNC)
            {
                real_index = (size_t) * (WORD*)(imexp->AddressOfNameOrdinals + 2 * index + (size_t)k32h.K32Handle);
                getpadd_addr = *(DWORD*)(imexp->AddressOfFunctions + 4 * real_index + (size_t)k32h.K32Handle) + (size_t)k32h.K32Handle;
                total_found++;
            }

        if (index >= imexp->NumberOfNames)
            break;
    }
    k32h.LoadLibraryAddress = (LOADLIBRARY)loadlib_addr;
    k32h.GetProcAddress = (GETPROCADDRESS)getpadd_addr;
    k32h.NTDHandle = (void*)k32h.LoadLibraryAddress(strings[26]);
    k32h.ADVHandle = (void*)k32h.LoadLibraryAddress(strings[27]);
    k32h.USRHandle = (void*)k32h.LoadLibraryAddress(strings[28]);
}

void RefreshAPIs(APIInterface& capi, Kernel32Access k32h, char strings[30][50])
{
    capi.CloseHandle = (CLOSEHANDLE)k32h.GetProcAddress(k32h.K32Handle, strings[0]);
    capi.VirtualAlloc = (VIRTUALALLOC)k32h.GetProcAddress(k32h.K32Handle, strings[1]);
    capi.VirtualFree = (VIRTUALFREE)k32h.GetProcAddress(k32h.K32Handle, strings[2]);
    capi.VirtualQueryEx = (VIRTUALQUERYEX)k32h.GetProcAddress(k32h.K32Handle, strings[3]);
    capi.CreateFile = (CREATEFILE)k32h.GetProcAddress(k32h.K32Handle, strings[4]);
    capi.ReadFile = (READFILE)k32h.GetProcAddress(k32h.K32Handle, strings[5]);
    capi.WriteFile = (WRITEFILE)k32h.GetProcAddress(k32h.K32Handle, strings[6]);
    capi.CreateThread = (CREATETHREAD)k32h.GetProcAddress(k32h.K32Handle, strings[7]);
    capi.CrateFileMaping = (CREATEFILEMAPPING)k32h.GetProcAddress(k32h.K32Handle, strings[8]);
    capi.MapViewOfFile = (MAPVIEWOFFILE)k32h.GetProcAddress(k32h.K32Handle, strings[9]);
    capi.UnmapViewOfFile = (UNMAPVIEWOFFILE)k32h.GetProcAddress(k32h.K32Handle, strings[10]);
    capi.Sleep = (SLEEP)k32h.GetProcAddress(k32h.K32Handle, strings[11]);
    capi.GetLocalTime = (GETLOCALTIME)k32h.GetProcAddress(k32h.K32Handle, strings[12]);
    capi.SetFilePointer = (SETFILEPOINTER)k32h.GetProcAddress(k32h.K32Handle, strings[13]);
    capi.GetFileSize = (GETFILESIZE)k32h.GetProcAddress(k32h.K32Handle, strings[14]);
    capi.QueryPerformanceCounter = (QUERYPERFORMANCECOUNTER)k32h.GetProcAddress(k32h.K32Handle, strings[15]);
    capi.QueryPerformanceFrequency = (QUERYPERFORMANCEFREQUENCY)k32h.GetProcAddress(k32h.K32Handle, strings[16]);
    capi.MessageBox = (MESSAGEBOX)k32h.GetProcAddress(k32h.USRHandle, strings[17]);
    capi.WaitForSingleObject = (WAITFORSINGLEOBJECT)k32h.GetProcAddress(k32h.K32Handle, strings[18]);
    capi.WaitForMultipleObjects = (WAITFORMULTIPLEOBJECTS)k32h.GetProcAddress(k32h.K32Handle, strings[19]);
    capi.GetStdHandle = (GETSTDHANDLE)k32h.GetProcAddress(k32h.K32Handle, strings[21]);
    capi.sprintf = (SPRINTF)k32h.GetProcAddress(k32h.NTDHandle, strings[22]);
    capi.strlen = (STRLEN)k32h.GetProcAddress(k32h.NTDHandle, strings[23]);
}

int ThreadFunction(void* param)
{
    Kernel32Access k32h;
    M_LDR_DATA_TABLE_ENTRY* table_entry_scanner;
#ifdef _M_X64
    IMAGE_NT_HEADERS64* nthdr;
#else
    IMAGE_NT_HEADERS* nthdr;
#endif
    IMAGE_EXPORT_DIRECTORY* imexp;
    size_t AddressOfNames = 0;
    size_t AddressOfNamesVA;
    size_t index = 0;
    char* fname;
    DWORD namehash = 0;
    size_t real_index;
    size_t loadlib_addr = 0;
    size_t getpadd_addr = 0;
    size_t total_found = 0;
    int i;

    M_PEB* peb = 0;
    __asm {
        mov eax, dword ptr fs : [30h]
        mov peb, eax
    }

    table_entry_scanner = (M_LDR_DATA_TABLE_ENTRY*)peb->LoaderData->loadOrder.flink;
    for (i = 0; table_entry_scanner->BaseDLLName[i]; i++)
        k32h.appname[i] = (char)table_entry_scanner->BaseDLLName[i];
    k32h.appname[i] = 0;
    while (table_entry_scanner)
    {
        wchar_t* sl = table_entry_scanner->BaseDLLName;
        wchar_t* sr = (wchar_t*)strings[29];
        int i;
        for (i = 0; sl[i]; i++)
            ;
            int l = i;
            for (i = 0; sr[i]; i++)
                ;
            int r = i;
            int result = 0;

            if (l < r)
                result = -1;
            else
                if (l > r)
                    result = 1;
                else
                {
                    int m = (l <= r) ? l : r;
                    for (int i = 0; i < m; i++)
                    {
                        wchar_t lc = (wchar_t)(sl[i] | 0x20);
                        wchar_t rc = (wchar_t)(sr[i] | 0x20);
                        if (lc < rc)
                        {
                            result = -1;
                            break;
                        }
                        else
                            if (lc > rc)
                            {
                                result = 1;
                                break;
                            }
                    }
                }

            //if (!strcmpic(table_entry_scanner->BaseDLLName, (wchar_t*)strings[29]))
            if (!result)
            {
                k32h.K32Handle = (void*)table_entry_scanner->BaseAddress;
                break;
            }
            table_entry_scanner = (M_LDR_DATA_TABLE_ENTRY*)table_entry_scanner->loadOrder.flink;
    }

    nthdr = (IMAGE_NT_HEADERS*)(((IMAGE_DOS_HEADER*)k32h.K32Handle)->e_lfanew + (size_t)k32h.K32Handle);
    imexp = (IMAGE_EXPORT_DIRECTORY*)(nthdr->OptionalHeader.DataDirectory[0].VirtualAddress + (size_t)k32h.K32Handle);

    AddressOfNames = imexp->AddressOfNames;
    AddressOfNamesVA = AddressOfNames + (size_t)k32h.K32Handle;

    for (i = 0; i < (int)imexp->NumberOfNames; i++)
    {
        AddressOfNamesVA += 4;
        index++;
        fname = (char*)(*(DWORD*)AddressOfNamesVA + (size_t)k32h.K32Handle);
        namehash = 0;
        for (i = 0; fname[i]; i++)
        {
            namehash ^= fname[i];
            namehash = (namehash << 8) | (namehash >> 24);
        }

        if (namehash == LOADLIB_FUNC)
        {
            real_index = (size_t) * (WORD*)(imexp->AddressOfNameOrdinals + 2 * index + (size_t)k32h.K32Handle);
            loadlib_addr = *(DWORD*)(imexp->AddressOfFunctions + 4 * real_index + (size_t)k32h.K32Handle) + (size_t)k32h.K32Handle;
            total_found++;
        }
        else
            if (namehash == GETPADR_FUNC)
            {
                real_index = (size_t) * (WORD*)(imexp->AddressOfNameOrdinals + 2 * index + (size_t)k32h.K32Handle);
                getpadd_addr = *(DWORD*)(imexp->AddressOfFunctions + 4 * real_index + (size_t)k32h.K32Handle) + (size_t)k32h.K32Handle;
                total_found++;
            }

        if (index >= imexp->NumberOfNames)
            break;
    }
    k32h.LoadLibraryAddress = (LOADLIBRARY)loadlib_addr;
    k32h.GetProcAddress = (GETPROCADDRESS)getpadd_addr;
    k32h.NTDHandle = (void*)k32h.LoadLibraryAddress(strings[26]);
    k32h.ADVHandle = (void*)k32h.LoadLibraryAddress(strings[27]);
    k32h.USRHandle = (void*)k32h.LoadLibraryAddress(strings[28]);
}

int main()
{
    char outp[100];
    DWORD write;
    APIInterface capi;
    Kernel32Access k32h;
    InitializeStrings(strings);

    BringMeFullAccess(k32h, strings);
    RefreshAPIs(capi, k32h, strings);

    void* std_out = capi.GetStdHandle(STD_OUTPUT_HANDLE);

    BYTE* buffers[MAX_THREADS];
    void* handles[MAX_THREADS];
    int k = 0;

    //Create 30000 Threads
    for (int i = 0; i < MAX_THREADS; i++)
    {
        BYTE* buffer = (BYTE*)capi.VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (!buffer)
            continue;
        for (int j = 0; j < 0x3C0; j++)
            buffer[j] = ((BYTE*)ThreadFunction)[j];
        handles[i] = capi.CreateThread(0, 0, (THREAD_START_ROUTINE*)buffer, 0, 0, 0);
        buffers[k++] = buffer;        
        if ((i % 100) == 0)
        {
            for (int j = 0; j < 100; j++)
                outp[j] = 0;
            //capi.sprintf(outp, "%d\n", i);
            //capi.WriteFile(std_out, outp, capi.strlen(outp), &write, 0);
        }
    }

    capi.WaitForMultipleObjects(MAX_THREADS, handles, TRUE, INFINITE);

    for (int j = 0; j < k; j++)
    {
        capi.VirtualFree(buffers[j], 0, MEM_RELEASE);
        buffers[j] = 0;
    }

    //a native execution should be able to create 2/3 of all buffers
    if (k > 20000)
        capi.sprintf(outp, "PASSED");        
    else
        capi.sprintf(outp, "FAILED");
    capi.WriteFile(std_out, outp, capi.strlen(outp), &write, 0);

    return 0;
}

{% endraw %}