{% raw %}
/*
ARCH: Windows x86
This example charts executable payloads and possible DLLs outside OS and compares the snapshot taken inside the instrumented process and outside of it through a child process
The test pass when there are no payloads and no dfference between inside and outside snapshots and fails otherwise
*/

#include <windows.h>
#include <Psapi.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <string>
#include <map>
using namespace std;

#pragma warning (disable:4996)

#define BUFFER_SIZE 0x1000000

struct MEM_INFO {
    size_t addr;
    size_t size;
    BYTE* buffer;
    bool allocated;
};

struct MEM_INFO_2 {
    size_t addr;
    size_t size;
    BYTE buffer[1];
};

struct SharedData {
    BOOL SYNC;
    BOOL READY;
    DWORD PID;
    MEM_INFO_2 data[1];
};

//mao existing memory ranges from current pid / parent pid
map<size_t, MEM_INFO> enumerate_memory(DWORD pid)
{
    map<size_t, MEM_INFO> regions;
    const PBYTE MaxAddress = (PBYTE)0x7FFFFFFF;
    HANDLE hProc = INVALID_HANDLE_VALUE;
    BOOL valid = FALSE;

    if (pid)
    {
        HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (hProc != INVALID_HANDLE_VALUE)
            valid = TRUE;
    }

    PBYTE addr = 0;
    while (addr < MaxAddress)
    {
        MEMORY_BASIC_INFORMATION mbi = { 0 };
        if (!pid)
        {
            if (VirtualQuery(addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) <= 0)
                break;
            if ((mbi.Protect == PAGE_EXECUTE_READ) || (mbi.Protect == PAGE_EXECUTE_READWRITE) || (mbi.Protect == PAGE_EXECUTE_WRITECOPY))
                regions[(size_t)mbi.BaseAddress] = {(size_t)mbi.BaseAddress, mbi.RegionSize, (BYTE*)mbi.BaseAddress, false};
        }
        else
        {
            if (valid)
            {
                if (VirtualQueryEx(hProc, addr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) <= 0)
                    break;                
                if ((mbi.Protect == PAGE_EXECUTE_READ) || (mbi.Protect == PAGE_EXECUTE_READWRITE) || (mbi.Protect == PAGE_EXECUTE_WRITECOPY))
                {
                    SIZE_T read;
                    BYTE* mem = (BYTE*)malloc(mbi.RegionSize);
                    ReadProcessMemory(hProc, mbi.BaseAddress, mem, mbi.RegionSize, &read);
                    regions[(size_t)mbi.BaseAddress] = { (size_t)mbi.BaseAddress, mbi.RegionSize, (BYTE*)mbi.BaseAddress, true };
                }
            }
        }
        
        addr += mbi.RegionSize;
    }

    if (valid)
        CloseHandle(hProc);

    return regions;
}

//format the memory ranges in shared mem, as vector<MEM_INFO>
map<size_t, MEM_INFO> ConstructResultFromSharedMemData(SharedData* binary_data)
{
    map<size_t, MEM_INFO> result;
    MEM_INFO_2* traverser = binary_data->data;

    while (traverser->size)
    {
        //printf("offset: %p, size: %08X\n", traverser, traverser->size);
        BYTE* replica = (BYTE*)malloc(traverser->size);
        memcpy(replica, traverser->buffer, traverser->size);
        result[traverser->addr] = { traverser->addr, traverser->size, replica, true };
        traverser = (MEM_INFO_2*)((BYTE*)traverser + traverser->size + 2 * sizeof(size_t));
    }

    return result;
}

//create cild process to read our memory ranges and provide them back
map<size_t, MEM_INFO> CreateProcessAndFedBackMyOwnMemoryRanges(const char* app_name)
{
    map<size_t, MEM_INFO> feedback;

    //create shared_mem
    HANDLE hShare = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFFER_SIZE, "Local\\FEEDBACK");
    if ((hShare == NULL) || (hShare == INVALID_HANDLE_VALUE))
    {
        printf("Can't create file mapping object: 0x%08X!\n", GetLastError());
        hShare = 0;
        return feedback;
    }
    SharedData* binary_data = (SharedData*)MapViewOfFile(hShare, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE);
    if (!binary_data)
    {
        printf("Unable to map view of file: 0x%08X\n", GetLastError());
        CloseHandle(hShare);
        hShare = 0;
        return feedback;
    }
    memset(binary_data, 0, BUFFER_SIZE);
    binary_data->READY = FALSE;

    //create child process
    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFO si = { 0 };
    char cmdline[MAX_PATH] = { 0 };
    sprintf(cmdline, "%s child", app_name);
    si.cb = sizeof(si);
    if (!CreateProcess(0, cmdline, 0, 0, FALSE, CREATE_DEFAULT_ERROR_MODE | CREATE_NEW_CONSOLE, 0, 0, &si, &pi))
        return feedback;

    //share my PID
    binary_data->PID = GetCurrentProcessId();
    binary_data->READY = TRUE;      //prepare child to read PID

    while (binary_data->READY)      //wait child to read PID
        Sleep(50);

    //wait for data    
    while (!binary_data->READY)     //wait for child to create our own structured memory ranges
        Sleep(50);

    //construct result from binary structures
    binary_data->READY = FALSE;

    feedback = ConstructResultFromSharedMemData(binary_data);
    //release shared_mem
    binary_data->READY = TRUE;      //inform child that we are done

    UnmapViewOfFile(binary_data);
    CloseHandle(hShare);

    return feedback;
}

//scan modules and payloads for the current process
int CheckMemoryRangesFromInside(const char* app_name, map<size_t, MEM_INFO> inside_snapshot)
{
    HMODULE moduleHandle;
    DWORD len;
    char moduleName[MAX_PATH] = { 0 };
    char* crt_fname;
    int extra_mem = 0;
    map<size_t, size_t> range;
    DWORD wow64_start = 0;
    DWORD wow64_size = 0;

    if (strstr(app_name, "\\"))
        crt_fname = PathFindFileName(app_name);
    else
        crt_fname = (char*)app_name;

    for (auto it : inside_snapshot)
    {
        if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (TCHAR*)it.first, &moduleHandle) == TRUE)
        {
            if ((len = GetMappedFileName(GetCurrentProcess(), moduleHandle, moduleName, MAX_PATH)) > 0)
            {
                if (strstr(moduleName, crt_fname))
                    continue;
                if (strstr(moduleName, "Windows") || strstr(moduleName, "windows"))
                    continue;
                printf("[inside-module]: [ADDR: %08X] [SIZE=%08X] [%s]\n", it.first, it.second.addr, moduleName);
                extra_mem++;
            }
        }
        else
        {
            //exclude x64 module wow64cpu.dll in x86 bit process
            MEMORY_BASIC_INFORMATION mb = { 0 };
            MEMORY_BASIC_INFORMATION mb2 = { 0 };
            VirtualQuery((void*)it.first, &mb, sizeof(mb));
            VirtualQuery(mb.AllocationBase, &mb2, sizeof(mb2));
            if (mb2.Protect && mb2.State != MEM_FREE)
            {
                if (*(WORD*)mb2.BaseAddress == 'ZM')
                {
                    DWORD addr = (DWORD)mb2.BaseAddress;
                    DWORD e_lfa = *(DWORD*)(addr + 0x3C);
                    WORD arch = *(WORD*)(addr + e_lfa + 4);
                    if (arch == 0x8664)
                    {
                        wow64_start = addr;
                        //IMAGE_SIZE
                        wow64_size = *(WORD*)(addr + e_lfa + 0x50);
                        continue;
                    }
                    else
                    {
                        extra_mem++;
                        printf("[inside-module-payload]: [ADDR: %08X] [SIZE=%08X]\n", it.first, it.second.size);
                    }
                }
                else
                {
                    if (it.first >= wow64_start && it.first < (wow64_start + wow64_size))
                        continue;
                    extra_mem++;
                    printf("[inside-payload]: [ADDR: %08X] [SIZE=%08X]\n", it.first, it.second.size);
                }
            }
        }
    }

    //memory ranges are only in set of {OS libraries, this module}
    if (!extra_mem)
        return 0;   

    //1 otherwise
    return 1;
}

//compare memory maps
int CheckMemoryRangesFromOutside(map<size_t, MEM_INFO> inside_snapshot, map<size_t, MEM_INFO> outside_snapshot)
{
    //outside \= inside
    for (auto it : inside_snapshot)
        outside_snapshot.erase(it.first);

    //outside \ inside == empty_set
    if (outside_snapshot.empty())
        return 0;

    printf("The following differences were found from outside (something inside the parent PID is cheating you !!!):\n");
    for (auto it : outside_snapshot)
        printf("[outside]: [ADDR: %08X] [SIZE=%08X]\n", it.first, it.second.size);

    //2 otherwise
    return 2;
}

//main flow for parent process
int ExecuteDBICheck(const char* app_name)
{
    //initial snapshot with memory ranges of current process, maybe as it is, maybe what DBI wanted us to see
    auto inside_snapshot = enumerate_memory(0);

    //snapshot with memory ranges seen outside the process, from a child process
    auto outside_snapshot = CreateProcessAndFedBackMyOwnMemoryRanges(app_name);

    //check 1 - this process should only contain this main module and OS libraries, no other payloads, no other hidden modules, but DBI may cheat us
    int flag1 = CheckMemoryRangesFromInside(app_name, inside_snapshot);

    //check 2 - compare inside_snapshot with outside_snapshot
    int flag2 = CheckMemoryRangesFromOutside(inside_snapshot, outside_snapshot);

    return flag1 | flag2;
}

//executes from the child process (feedback to parent process, it's own memory ranges, using a shared memory)
void ExecuteFeedbackProvider()
{
    //map shared mem
    HANDLE hShare = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFFER_SIZE, "Local\\FEEDBACK");
    if (hShare == NULL || hShare == INVALID_HANDLE_VALUE)
        return;
    SharedData* binary_data = (SharedData*)MapViewOfFile(hShare, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE);
    if (!binary_data)
    {
        CloseHandle(hShare);
        return;
    }

    //read parent PID
    DWORD pid;
    while (!binary_data->READY)               //wait for child to create our own structured memory ranges
        Sleep(50);

    pid = binary_data->PID;               //read PID
    binary_data->READY = FALSE;           //block parent till memory ranges are read
    Sleep(100);

    //read parent process memory ranges
    auto outside_snapshot = enumerate_memory(pid);

    //transform data to low-level data and write to shared mem
    BYTE* offset = (BYTE*)binary_data->data;
    for (auto it : outside_snapshot)
    {
        //printf("offset: %p, size: %08X\n", offset, it.second.size);
        MEM_INFO_2* current = (MEM_INFO_2*)offset;
        current->addr = it.second.addr;
        current->size = it.second.size;
        memcpy(current->buffer, it.second.buffer, it.second.size);
        offset += 2 * sizeof(size_t) + it.second.size;
    }
    binary_data->READY = TRUE;
    Sleep(100);

    //wait for parent to process
    while (!binary_data->READY)     //wait for child to create our own structured memory ranges
        Sleep(50);

    //unmap shared mem
    UnmapViewOfFile(binary_data);
    CloseHandle(hShare);
}

int main(int argc, char* argv[])
{
    int result; 

	if (argc == 1)
        //parent execution
		result = ExecuteDBICheck(argv[0]);
	else
		if (!strcmp(argv[1], "child"))
            //cild execution
			ExecuteFeedbackProvider();

    if (result > 0)
        printf("FAILED");
    else
        printf("PASSED");

	return 0;
}
{% endraw %}