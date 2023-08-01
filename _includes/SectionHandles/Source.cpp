{% raw %}
/*
ARCH: Windows x86
This example query object handles of type HANDLE_SECTION systemwide and attempts to identify common DBI resources inside the shared memory region defined by the section
Both PIN and DynamoRIO use a shared SECTION
*/

#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <shlwapi.h>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

#pragma warning (disable:4996)

#define SystemHandleInformation 16
#define ObjectBasicInformation 0
#define ObjectNameInformation 1
#define ObjectTypeInformation 2

#define STATUS_INFO_LENGTH_MISMATCH 0xC0000004
#define STATUS_SUCCESS 0

#define HANDLE_DIRECTORY    3
#define HANDLE_PROCESS      7
#define HANDLE_DEBUGOB      15
#define HANDLE_EVENT        16
#define HANDLE_MUTANT       17
#define HANDLE_SEMAPHORE    19
#define HANDLE_FILE         37
#define HANDLE_SECTION      42
#define HANDLE_KEY          44
#define HANDLE_RPC_PORT     46

typedef DWORD(WINAPI* PNtQueryObject)(HANDLE, DWORD, VOID*, DWORD, VOID*);
typedef DWORD(WINAPI* PNtQuerySystemInformation)(DWORD, VOID*, DWORD, ULONG*);

#define MAX_UNICODE_PATH	32767L

struct UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
};

struct OBJECT_TYPE_INFORMATION {
    UNICODE_STRING          TypeName;
    ULONG                   TotalNumberOfHandles;
    ULONG                   TotalNumberOfObjects;
    WCHAR                   Unused1[8];
    ULONG                   HighWaterNumberOfHandles;
    ULONG                   HighWaterNumberOfObjects;
    WCHAR                   Unused2[8];
    ACCESS_MASK             InvalidAttributes;
    GENERIC_MAPPING         GenericMapping;
    ACCESS_MASK             ValidAttributes;
    BOOLEAN                 SecurityRequired;
    BOOLEAN                 MaintainHandleCount;
    USHORT                  MaintainTypeList;
    int                     PoolType;
    ULONG                   DefaultPagedPoolCharge;
    ULONG                   DefaultNonPagedPoolCharge;
};

struct OBJECT_NAME_INFORMATION {
    UNICODE_STRING          Name;
    WCHAR                   NameBuffer[1];
};

struct SYSTEM_HANDLE
{
    ULONG ProcessId;
    BYTE ObjectTypeNumber;
    BYTE Flags;
    USHORT Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
};

struct SYSTEM_HANDLE_INFORMATION
{
    DWORD           Count;
    SYSTEM_HANDLE   Handles[1];
};

//------------------------------------------------------------------------------------------------

//allow access to other objects inside the OS
BOOL EnableDebugPriv(void)
{
    HANDLE hToken;
    LUID sedebugnameValue;
    LUID sebackupname;
    TOKEN_PRIVILEGES tkp;

    // enable the SeDebugPrivilege
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return FALSE;

    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
    {
        CloseHandle(hToken);
        return FALSE;
    }

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sedebugnameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL);

    if (!LookupPrivilegeValue(NULL, SE_BACKUP_NAME, &sebackupname))
    {
        CloseHandle(hToken);
        return FALSE;
    }

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sebackupname;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL);

    CloseHandle(hToken);
    return TRUE;
}

//describe handle
BOOL GetTypeToken(HANDLE h, WCHAR* str, WCHAR* objname, DWORD processId, BYTE obj_index, const char* proc_name)
{
    ULONG size = 0;
    BYTE* lpBuffer = 0;
    BOOL ret = FALSE;
    HANDLE handle = 0;
    HANDLE hRemoteProcess = NULL;
    DWORD x_pid = GetCurrentProcessId();
    BOOL remote = processId != x_pid;
    PNtQueryObject NtQueryObject;
    CHAR target_pname[MAX_PATH] = { 0 };
    NTSTATUS status;
    DWORD dummy = 0;
    void* base_address = 0;
    LARGE_INTEGER offset = { 0 };
    BYTE* mem = 0;
    char extra_info[100] = { 0 };

    ZeroMemory(str, MAX_PATH);

    if (remote)
    {
        hRemoteProcess = OpenProcess(PROCESS_DUP_HANDLE, TRUE, processId);

        if (hRemoteProcess == NULL)
            return FALSE;

        DuplicateHandle(hRemoteProcess, h, GetCurrentProcess(), &handle, 0, FALSE, DUPLICATE_SAME_ACCESS);
        if (!handle)
            return FALSE;
    }
    else
        handle = h;

    NtQueryObject = (PNtQueryObject)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryObject");

    status = NtQueryObject(handle, ObjectTypeInformation, &dummy, sizeof(dummy), &size);
    if (STATUS_INFO_LENGTH_MISMATCH != status && status != STATUS_SUCCESS)
    {
        if (remote)
        {
            if (hRemoteProcess != NULL)
                CloseHandle(hRemoteProcess);

            if (handle != NULL)
                CloseHandle(handle);
        }
        return FALSE;
    }
    lpBuffer = (BYTE*)malloc(size);
    memset(lpBuffer, 0, size);
    NtQueryObject(handle, ObjectTypeInformation, lpBuffer, size, &size);
    OBJECT_TYPE_INFORMATION* pObjectTypeInfo = (OBJECT_TYPE_INFORMATION*)lpBuffer;
    wcscpy(str, pObjectTypeInfo->TypeName.Buffer);
    free(lpBuffer);
    lpBuffer = 0;

    status = NtQueryObject(handle, ObjectNameInformation, &dummy, sizeof(dummy), &size);
    if (STATUS_INFO_LENGTH_MISMATCH != status && status != STATUS_SUCCESS)
    {
        if (remote)
        {
            if (hRemoteProcess != NULL)
                CloseHandle(hRemoteProcess);

            if (handle != NULL)
                CloseHandle(handle);
        }
        return FALSE;
    }
    lpBuffer = (BYTE*)malloc(size);
    memset(lpBuffer, 0, size);
    NtQueryObject(handle, ObjectNameInformation, lpBuffer, size, &size);   
    OBJECT_NAME_INFORMATION* pObjectNameInfo = (OBJECT_NAME_INFORMATION*)lpBuffer;
    if (pObjectNameInfo->Name.Buffer)
        wcscpy(objname, pObjectNameInfo->Name.Buffer);
    else
        if (obj_index == HANDLE_PROCESS)
            GetModuleFileNameExW(handle, NULL, objname, MAX_PATH);
        else
            if (obj_index == HANDLE_SECTION)
            {
                //extract lib name from the data content inside the shared section of DynamoRIO
                if (strstr(proc_name, "drrun.exe"))
                {
                    mem = (BYTE*)MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0x130000);
                    if (mem)
                    {
                        IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)mem;
                        if (imdos->e_magic == 0x5a4d && imdos->e_lfanew < 0x1000)
                        {
                            IMAGE_NT_HEADERS* imnt = (IMAGE_NT_HEADERS*)(mem + imdos->e_lfanew);
                            if (imnt->FileHeader.Machine == 0x14C)
                            {
                                DWORD export_rva = imnt->OptionalHeader.DataDirectory[0].VirtualAddress;
                                DWORD lib_name_rva = *(DWORD*)(mem + export_rva + 12);
                                swprintf(objname, L"OK:%p:%S", mem, (char*)(mem + lib_name_rva));
                            }
                            else
                            {
                                IMAGE_NT_HEADERS64* imnt64 = (IMAGE_NT_HEADERS64*)(mem + imdos->e_lfanew);
                                //to do
                            }
                        }
                        UnmapViewOfFile(mem);
                    }
                    else
                        swprintf(objname, L"FAIL:%d", GetLastError());
                }
                else
                    //extract path to pinvm.dll from the data content inside the shared section of PIN
                    if (strstr(proc_name, "pin.exe"))
                    {
                        mem = (BYTE*)MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0x1000);

                        if (mem)
                        {
                            char fname[100];
                            DWORD write;
                            sprintf(fname, "pin_dump_%08X.bin", mem);
                            HANDLE f = CreateFile(fname, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                            WriteFile(f, mem, 0x1000, &write, NULL);
                            CloseHandle(f);

                            char to_match[] = ":\\";
                            size_t i = 0;
                            while (i < 0x1000 - strlen(to_match))
                            {
                                if (!strncmp((char*)(mem + i), to_match, strlen(to_match)))
                                {
                                    swprintf(objname, L"OK:%p:%S", mem, mem + i - 1);
                                    break;
                                }
                                i++;
                            }                            
                        }
                    }
            }

    free(lpBuffer);
    lpBuffer = 0;

    if (remote)
    {
        if (hRemoteProcess != NULL)
            CloseHandle(hRemoteProcess);

        if (handle != NULL)
            CloseHandle(handle);
    }

    return TRUE;
}

//check if access should not be considered
bool IsAccessInBlackList(DWORD access)
{
    static DWORD blacklist_access[4] = { 0x90000cc2, 0x0012019f, 0x00120189, 0x001A019F };
    for (int j = 0; j < 4; j++)
    {
        if (blacklist_access[j] == access)
            return true;
    }
    return false;
}

//check if handle is in a range of accepted handles
bool IsAcceptedHandleType(DWORD handle_type)
{
    static set<DWORD> handle_types = { HANDLE_DIRECTORY, HANDLE_PROCESS, HANDLE_DEBUGOB, HANDLE_EVENT, HANDLE_MUTANT, HANDLE_SEMAPHORE, HANDLE_FILE, HANDLE_SECTION, HANDLE_KEY, HANDLE_RPC_PORT };
    return handle_types.find(handle_type) != handle_types.end();
}

//for debug purpose, list handles by type and owner process
void ListHandles(map<DWORD, map<DWORD, set<string>>>& handles, map<DWORD, string>& procnames)
{
    map<DWORD, string> obj_types = {
        {HANDLE_DIRECTORY, "HANDLE_DIRECTORY"},
        {HANDLE_PROCESS  , "HANDLE_PROCESS"},
        {HANDLE_DEBUGOB  , "HANDLE_DEBUGOB"},
        {HANDLE_EVENT    , "HANDLE_EVENT"},
        {HANDLE_MUTANT   , "HANDLE_MUTANT"},
        {HANDLE_SEMAPHORE, "HANDLE_SEMAPHORE"},
        {HANDLE_FILE     , "HANDLE_FILE"},
        {HANDLE_SECTION  , "HANDLE_SECTION"},
        {HANDLE_KEY      , "HANDLE_KEY"},
        {HANDLE_RPC_PORT , "HANDLE_RPC_PORT"}
    };

    printf("PID: %d\n", GetCurrentProcessId());
    for (auto it : handles)
    {
        int total = 0;
        for (auto it2 : it.second)
            total += it2.second.size();
        printf("%s : %d\n", obj_types[it.first].c_str(), total);
    }

    for (auto it : handles)
    {
        static char fname[0x1000] = { 0 };
        DWORD write;
        sprintf(fname, "_handles_%s.txt", obj_types[it.first].c_str());
        HANDLE f = CreateFile(fname, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        for (auto it2 : it.second)
        {
            sprintf(fname, "[%d] : [%s]\n", it2.first, procnames[it2.first].c_str());
            WriteFile(f, fname, strlen(fname), &write, 0);
            for (auto it3 : it2.second)
            {
                sprintf(fname, "  [%s]\n", it3.c_str());
                WriteFile(f, fname, strlen(fname), &write, 0);
            }
        }
        CloseHandle(f);
    }
}

//check if the passed param is an OS related process, list may be larger
bool IsOSProcess(string pname)
{
    set<string> elements = { "conhost.exe", "svchost.exe", "cmd.exe", "far.exe", "explorer.exe", "tcmd.exe" };
    for (auto it : elements)
    {
        if (pname.find(it) != string::npos)
            return true;
    }
    return false;
}

//return the pid of a parent having an opened handle to our process, if the parent is not an OS related process
bool SearchForSectionHandles(map<DWORD, map<DWORD, set<string>>>& handles, map<DWORD, string>& procnames)
{
    HMODULE hMod;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (char*)&ListHandles, &hMod);
    char cmodule[MAX_PATH] = { 0 };    
    GetModuleFileName(hMod, cmodule, MAX_PATH);    
    string low_src = PathFindFileName(cmodule);
    transform(low_src.begin(), low_src.end(), low_src.begin(), ::tolower);
    set<DWORD> potential_dbis;
    static vector<string> blacklist_DBIs = { "coai.exe", "drrun.exe", "flow.exe", "qbdi.exe", "pin.exe", "dynamorio", "pinvm.dll"};

    //populate set of processes having process handle to current process
    for (auto it : handles)
    {
        if (it.first == HANDLE_PROCESS)
        {
            for (auto it2 : it.second)
            {
                if (it2.first != GetCurrentProcessId())
                {
                    for (auto it3 : it2.second)
                    {
                        if (!it3.empty())
                        {                            
                            if (it3.find(low_src) != string::npos && !IsOSProcess(procnames[it2.first]))
                                potential_dbis.insert(it2.first);
                        }
                    }
                }
            }
        }
    }

    for (auto it : handles)
    {
        if (it.first == HANDLE_SECTION)
        {
            for (auto it2 : potential_dbis)
            {
                for (auto it3 : it.second[it2])
                {
                    if (!it3.empty())
                    {
                        printf("SECTION: [%s]\n", it3.c_str());
                        for (auto it4 : blacklist_DBIs)
                        {
                            if (it3.find(it4) != string::npos)
                                return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

int main()
{
    DWORD size = 0x2000;
    DWORD needed = 0;
    WCHAR strType[MAX_PATH];
    WCHAR objname[MAX_PATH * 2];
    SYSTEM_HANDLE_INFORMATION* pSysHandleInformation;
    PNtQuerySystemInformation NtQuerySystemInformation;
    map<DWORD, string> procnames;
    map<DWORD, map<DWORD, set<string>>> handles;    
    
    //allow process to query other objects
    EnableDebugPriv();

    //build a list of all possible OS Handles 
    pSysHandleInformation = (SYSTEM_HANDLE_INFORMATION*)malloc(size);
    if (!pSysHandleInformation)
        return FALSE;
    NtQuerySystemInformation = (PNtQuerySystemInformation)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQuerySystemInformation");
    if (NtQuerySystemInformation(SystemHandleInformation, pSysHandleInformation, size, &needed) != 0)
    {
        if (!needed)
        {
            free(pSysHandleInformation);
            return FALSE;
        }
        free(pSysHandleInformation);
        size = needed * 2;
        pSysHandleInformation = (SYSTEM_HANDLE_INFORMATION*)malloc(size);
        if (pSysHandleInformation == NULL)
            return FALSE;
    }
    if (NtQuerySystemInformation(16, pSysHandleInformation, size, NULL))
    {
        free(pSysHandleInformation);
        return FALSE;
    }

    //map accepted handles to [handles] and describe handle name and parent process
    for (int i = 0; i < pSysHandleInformation->Count; i++)
    {
        //ignore some blocking access rights, there may be more on other Windows versions
        if (!IsAccessInBlackList(pSysHandleInformation->Handles[i].GrantedAccess))
        {
            memset(strType, 0, sizeof(strType));
            memset(objname, 0, sizeof(objname));   

            if (procnames.find(pSysHandleInformation->Handles[i].ProcessId) == procnames.end())
            {
                HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pSysHandleInformation->Handles[i].ProcessId);
                CHAR parentProcessPath[2 * MAX_PATH] = { 0 };
                if (hProc)
                {                    
                    if (GetModuleFileNameEx(hProc, NULL, parentProcessPath, MAX_PATH))
                    {
                        string pname = parentProcessPath;
                        transform(pname.begin(), pname.end(), pname.begin(), ::tolower);
                        //map parent process
                        procnames[pSysHandleInformation->Handles[i].ProcessId] = pname;
                    }
                    CloseHandle(hProc);
                }
            }
            //describe handle
            GetTypeToken((HANDLE)pSysHandleInformation->Handles[i].Handle, strType, objname, pSysHandleInformation->Handles[i].ProcessId, pSysHandleInformation->Handles[i].ObjectTypeNumber, procnames[pSysHandleInformation->Handles[i].ProcessId].c_str());
            //is handle accepted
            if (IsAcceptedHandleType(pSysHandleInformation->Handles[i].ObjectTypeNumber))
            {
                wstring wobjname = objname;
                if (!wobjname.empty())
                    transform(wobjname.begin(), wobjname.end(), wobjname.begin(), ::tolower);
                //map handle
                handles[pSysHandleInformation->Handles[i].ObjectTypeNumber][pSysHandleInformation->Handles[i].ProcessId].insert(string(wobjname.begin(), wobjname.end()));
            }
        }
    }

    //release the list of initial handles
    if (pSysHandleInformation)
        free(pSysHandleInformation);

    //ListHandles(handles, procnames);

    //search for DBIs shared sections
    if (SearchForSectionHandles(handles, procnames))
        printf("FAILED");
    else
        printf("PASSED");

	return 0;
}

{% endraw %}