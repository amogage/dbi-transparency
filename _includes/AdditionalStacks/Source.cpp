
/*
ARCH: Windows x86
This example attempts to search for memory regions having STACK properties and:
    - it passes when all possible stack pointers reference OS specific modules;
    - it fails when some of the stack pointers reference modules or payloads, that are not part of the OS modules

Stack properties:
    - memory does not start with bytes other than 0
    - memory has RW access rights
    - memory has at least 1 string ASCII/UTF16 of size>=10
    - memory is larger than 4KB in size

Whitelist modules:
    "main-module.exe",
    "msvcp...dll",
    "vcruntime...dll",
    "kernel32.dll",
    "kernelbase.dll",
    "ucrtbase.dll",
    "wow64cpu.dll",
    "ntdll.dll"

Each stack found, is traversed and all possible pointers are mapped to their corresponding source memory.
Any other module or payload is actually a DBI injected dll or a DBI code-cache
*/

#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <algorithm>
using namespace std;

#pragma warning (disable:4996)

#define MAX_MEMORY_x86 0x7FFFFFFF

/*
    structure describing potential modules or DBI - cache associated with a DBI and referenced from multiple STACK pointers
    name:       module name referenced by return-to pointers
    pointers:   total pointers referencing this module
    stack_ref:  start address of the stack, where pointers referencing this module were found
    size:       memory region size, used for mem-dump
*/
struct STACK_INFO {
    string name;
    set<BYTE*> pointers;
    BYTE* stack_ref;
    DWORD size;
};

//iterate memory regions - adapted from al-khaser
vector<MEMORY_BASIC_INFORMATION*> enumerate_memory()
{
    vector<MEMORY_BASIC_INFORMATION*> regions;
    const PBYTE MaxAddress = (PBYTE)MAX_MEMORY_x86;

    PBYTE addr = 0;
    while (addr < MaxAddress)
    {
        auto mbi = new MEMORY_BASIC_INFORMATION();
        if (VirtualQuery(addr, mbi, sizeof(MEMORY_BASIC_INFORMATION)) <= 0)
        {
            delete mbi;
            break;
        }

        if ((mbi->State != MEM_COMMIT) || (mbi->Protect != PAGE_READWRITE) || (mbi->BaseAddress == mbi->AllocationBase))
        {
            addr += mbi->RegionSize;
            delete mbi;
        }
        else
        {
            addr += mbi->RegionSize;
            regions.push_back(mbi);
        }        
    }

    return regions;
}

//Dump a memory region defined by start address and size
void DumpRegion(void* addr, SIZE_T size, const char* extension)
{
    DWORD write;
    char buffer[MAX_PATH] = { 0 };
    sprintf(buffer, "_%p_%X.%s", addr, size, extension);
    HANDLE f = CreateFile(buffer, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (f == INVALID_HANDLE_VALUE)
        return;
    if (WriteFile(f, addr, size, &write, 0))
        CloseHandle(f);
}

//Check if module is part of OS modules
bool IsInWhitelist(string name)
{
    static vector<string> whitelist = { "additionalstacks", "msvcp", "vcruntime","kernel","ucrtbase","wow64cpu","ntdll" };
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    for (auto it : whitelist)
    {
        if ((name.find(it) != string::npos) && (name.find(".dll") != string::npos || name.find(".exe") != string::npos))
            return true;
    }
    return false;
}

//Check if the memory region starting at addr has at least 1 string ASCII/UNICODE of size = minimum_string_size
bool CheckStringPresence(void* addr, SIZE_T size, int minimum_string_size)
{
    int i = 0;
    int j;
    char el_A;
    wchar_t el_W;

    while (i < size - minimum_string_size)
    {       
        if ((*(DWORD*)((char*)addr + i) & 0xFF00FF00) != 0)
        {
            for (j = 0; j < minimum_string_size; j++)
            {
                el_A = ((char*)addr + i)[j];
                if (el_A < ' ' || el_A > '~')
                {
                    i += j + 1;
                    break;
                }
            }
            if (j == minimum_string_size)
                return true;
        }
        else
            if (*(DWORD*)((char*)addr + i) != 0)
            {
                for (j = 0; j < minimum_string_size; j++)
                {
                    el_W = ((wchar_t*)((BYTE*)addr + i))[j];
                    if (el_W < L' ' || el_W > L'~')
                    {
                        i += (j + 1) * sizeof(wchar_t);
                        break;
                    }
                }
                if (j == minimum_string_size)
                    return true;
            }
            else
                i += sizeof(DWORD);
    }
    return false;
}

//scan mem for pointers redirecting to other memory areas and map these memory areas into stack_refs
void ChartPointers(MEMORY_BASIC_INFORMATION* mem, map<BYTE*, STACK_INFO>& stack_refs)
{
    map<string, pair<BYTE*, SIZE_T>> modules;
    DWORD* mem_base = (DWORD*)mem->BaseAddress;
    DWORD count = mem->RegionSize / sizeof(DWORD);

    for (int i = 0; i < count; i++)
    {
        BYTE* potential_ptr = (BYTE*)mem_base[i];
        if (!potential_ptr)
            continue;
        MEMORY_BASIC_INFORMATION pmem = { 0 };
        VirtualQuery(potential_ptr, &pmem, sizeof(pmem));
        if (pmem.State == MEM_FREE)
            continue;

        //we are interested only in executable memory pages
        if ((pmem.Protect == PAGE_EXECUTE_READ || pmem.Protect == PAGE_EXECUTE_READWRITE) && (pmem.RegionSize > 0x1000))
        {
            if (stack_refs.find((BYTE*)pmem.AllocationBase) == stack_refs.end())
            {
                HMODULE hmod = 0;
                char mod_fname[MAX_PATH] = { 0 };
                //attempt discovery of a possible module
                GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (char*)pmem.AllocationBase, &hmod);
                if (hmod)
                {
                    GetModuleFileName(hmod, mod_fname, MAX_PATH);
                    if (!mod_fname[0])
                        sprintf(mod_fname, "payload_%p", pmem.AllocationBase);
                }
                else
                    //if the module is not recognized but it starts with MZPE header, maybe a code-injection or payload
                    if (*(WORD*)pmem.AllocationBase == 'ZM')
                    {
                        IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)pmem.AllocationBase;
                        WORD arch = *(WORD*)(imdos->e_lfanew + (BYTE*)pmem.AllocationBase + 4);
                        //64 bit module in 32 bit process (maybe wow64cpu.dll)
                        if (arch == 0x8664)
                        {
                            IMAGE_NT_HEADERS64* imnt = (IMAGE_NT_HEADERS64*)(imdos->e_lfanew + (BYTE*)pmem.AllocationBase);
                            DWORD RVA_exports = imnt->OptionalHeader.DataDirectory[0].VirtualAddress;                            
                            IMAGE_SECTION_HEADER* imsec = (IMAGE_SECTION_HEADER*)((BYTE*)pmem.AllocationBase + imdos->e_lfanew + sizeof(IMAGE_NT_HEADERS64));
                            IMAGE_EXPORT_DIRECTORY* imexp = (IMAGE_EXPORT_DIRECTORY*)((BYTE*)pmem.AllocationBase + RVA_exports);
                            char* export_name = (char*)(((BYTE*)pmem.AllocationBase) + imexp->Name);
                            strcpy(mod_fname, export_name);
                        }
                        else
                        //payload x86
                        {
                            IMAGE_NT_HEADERS32* imnt = (IMAGE_NT_HEADERS32*)(imdos->e_lfanew + (BYTE*)pmem.AllocationBase);
                            DWORD RVA_exports = imnt->OptionalHeader.DataDirectory[0].VirtualAddress;
                            IMAGE_SECTION_HEADER* imsec = (IMAGE_SECTION_HEADER*)((BYTE*)pmem.AllocationBase + imdos->e_lfanew + sizeof(IMAGE_NT_HEADERS32));
                            IMAGE_EXPORT_DIRECTORY* imexp = (IMAGE_EXPORT_DIRECTORY*)((BYTE*)pmem.AllocationBase + RVA_exports);
                            char* export_name = (char*)(((BYTE*)pmem.AllocationBase) + imexp->Name);
                            strcpy(mod_fname, export_name);
                        }
                    }
                    else
                        //just a regular payload, without any kind of header
                        sprintf(mod_fname, "payload_%p", pmem.AllocationBase);

                //consider modules and payloads which are not part of the whitelist
                if (!IsInWhitelist(mod_fname) && modules.find(mod_fname) == modules.end())
                {
                    stack_refs[(BYTE*)pmem.AllocationBase].name = mod_fname;
                    stack_refs[(BYTE*)pmem.AllocationBase].pointers.insert(potential_ptr);
                    stack_refs[(BYTE*)pmem.AllocationBase].stack_ref = (BYTE*)mem->BaseAddress;
                    stack_refs[(BYTE*)pmem.AllocationBase].size = pmem.RegionSize;

                    modules[mod_fname] = { (BYTE*)pmem.AllocationBase, pmem.RegionSize};                    
                }
            }
            else
                stack_refs[(BYTE*)pmem.AllocationBase].pointers.insert(potential_ptr);
        }
    }
}

int main()
{
    map<BYTE*, MEMORY_BASIC_INFORMATION*> stacks;    
    map<BYTE*, STACK_INFO> stack_refs;
    int i = 0;

    //search for valid memory regions
    vector<MEMORY_BASIC_INFORMATION*> regions = enumerate_memory();

    //filter memory regions and apply stack properties
    for (auto it : regions)
    {
        MEMORY_BASIC_INFORMATION mb = { 0 };
        VirtualQuery(it->AllocationBase, &mb, sizeof(MEMORY_BASIC_INFORMATION));
        if ((mb.RegionSize > 0x1000) && (it->RegionSize > 0x1000) && !*(long long*)it->BaseAddress)
        {
            if (CheckStringPresence(it->BaseAddress, it->RegionSize, 10))
            {
                stacks[(BYTE*)it->BaseAddress] = it;
                //printf("%d. %p : %X|%X  |||  %p : %X|%X\n", i, it->BaseAddress, it->RegionSize, it->Protect, mb.BaseAddress, mb.RegionSize, mb.Protect);
                i++;
            }
        }
    }

    //cumulate and map pointers found in each stack, into stack_refs
    for (auto it : stacks)
        ChartPointers(it.second, stack_refs);

    //display the unexpected stack_refs found so far
    for (auto it : stack_refs)
    {
        printf("['%s':%p] : <pointers:%d> : <ref:%p>\n", it.second.name.c_str(), it.first, it.second.pointers.size(), it.second.stack_ref);
        //this is for debug purpose, uncomment if required
        //DumpRegion(it.first, it.second.size, "referenced_in_unexpected_stack");
        //DumpRegion(it.second.stack_ref, stacks[it.second.stack_ref]->RegionSize, "unexpected_stack");
    }

    //free regions
    for (auto it : regions)
        delete it;

    //no additional stacks should be found on native execution
    if (stack_refs.empty())
        printf("PASSED");
    else
        printf("FAILED");
   
    return 0;
}
