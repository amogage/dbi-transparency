{% raw %}
/*
ARCH: Windows x86/x64
This example iterates API detour hooks, there should be no hook at all in a native application
If at least 1 hook is found, the test fails
*/

#include <windows.h>
#include <stdio.h>

struct MZPE_SPECS {
    BYTE* sec0_VA;
    DWORD sec0_size;
};


HMODULE* SearchForLibs(size_t& count)
{
    MEMORY_BASIC_INFORMATION crt = { 0 };
    HMODULE found_total[1000] = { 0 };

#ifdef ENV32BIT
    const PBYTE MaxAddress = (PBYTE)0x7FFFFFFF;
#else
    const PBYTE MaxAddress = (PBYTE)0x7FFFFFFFFFFFFFFFULL;
#endif

    PBYTE addr = 0;
    size_t i = 0;

    while (addr < MaxAddress)
    {
        memset(&crt, 0, sizeof(crt));
        if (VirtualQuery(addr, &crt, sizeof(MEMORY_BASIC_INFORMATION)) <= 0)
            break;
        addr += crt.RegionSize;
        if (crt.State == MEM_FREE)
            continue;
        if (((crt.State & MEM_COMMIT) == MEM_COMMIT) && (crt.Protect == PAGE_READONLY))
        {
            if (*(WORD*)crt.BaseAddress == 0x5A4D)
            {
                IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)crt.BaseAddress;
                IMAGE_NT_HEADERS* imnt = (IMAGE_NT_HEADERS*)((BYTE*)crt.BaseAddress + imdos->e_lfanew);
                if (imnt->FileHeader.Characteristics & IMAGE_FILE_DLL)
                    found_total[i++] = (HMODULE)crt.BaseAddress;
            }
        }
    }

   count = i;

   HMODULE* result = (HMODULE*)malloc(count * sizeof(HMODULE));
   memset(result, 0, count * sizeof(HMODULE));
   memcpy(result, found_total, count * sizeof(HMODULE));

    return result;
}

void SearchMem()
{
    size_t total_count = 0;
    auto memoryRegions = SearchForLibs(total_count);
    printf("[%d] modules \n", total_count);

    printf("Search: ... \n");
    for (size_t i = 0; i < total_count; i++)
    {
        IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)memoryRegions[i];
        IMAGE_NT_HEADERS32* imnt32 = (IMAGE_NT_HEADERS32*)((BYTE*)memoryRegions[i] + imdos->e_lfanew);
        IMAGE_NT_HEADERS64* imnt64 = (IMAGE_NT_HEADERS64*)((BYTE*)memoryRegions[i] + imdos->e_lfanew);
        DWORD rva_exports;
        DWORD size;
        IMAGE_SECTION_HEADER* imsec = 0;
        DWORD header_size = 0;
        DWORD sections = 0;
        DWORD sec_align = 0;
        DWORD file_align = 0;
        if (imnt32->FileHeader.Machine == 0x14C)
        {
            rva_exports = imnt32->OptionalHeader.DataDirectory[0].VirtualAddress;
            size = imnt32->OptionalHeader.SizeOfImage;
            imsec = (IMAGE_SECTION_HEADER*)((BYTE*)imnt32 + sizeof(IMAGE_NT_HEADERS32));
            sec_align = imnt32->OptionalHeader.SectionAlignment;
            file_align = imnt32->OptionalHeader.FileAlignment;
            header_size = (imnt32->OptionalHeader.SizeOfHeaders / sec_align + 1) * sec_align;
            sections = imnt32->FileHeader.NumberOfSections;
        }
        else
        {
            rva_exports = imnt64->OptionalHeader.DataDirectory[0].VirtualAddress;
            size = imnt64->OptionalHeader.SizeOfImage;
            imsec = (IMAGE_SECTION_HEADER*)((BYTE*)imnt64 + sizeof(IMAGE_NT_HEADERS64));
            sec_align = imnt64->OptionalHeader.SectionAlignment;
            file_align = imnt64->OptionalHeader.FileAlignment;
            header_size = (imnt64->OptionalHeader.SizeOfHeaders / sec_align + 1) * sec_align;
            sections = imnt64->FileHeader.NumberOfSections;
        }
        DWORD name_rva = *(DWORD*)((BYTE*)memoryRegions[i] + rva_exports + 12);
        char* name = (char*)((BYTE*)memoryRegions[i] + name_rva);
        DWORD write = 0;
        DWORD sec_size = 0;
        HANDLE f = CreateFile(name, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        WriteFile(f, memoryRegions[i], size, &write, 0);
        CloseHandle(f);

    }
    free(memoryRegions);
}

MZPE_SPECS GetMZPESpecs(BYTE* buffer, bool mem)
{
    MZPE_SPECS result = { 0 };
    IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)buffer;
#ifndef _M_X64
    IMAGE_NT_HEADERS32* imnt = (IMAGE_NT_HEADERS32*)(buffer + imdos->e_lfanew);
    IMAGE_SECTION_HEADER* imsec = (IMAGE_SECTION_HEADER*)((BYTE*)imnt + sizeof(IMAGE_NT_HEADERS32));
#else
    IMAGE_NT_HEADERS64* imnt = (IMAGE_NT_HEADERS64*)(buffer + imdos->e_lfanew);
    IMAGE_SECTION_HEADER* imsec = (IMAGE_SECTION_HEADER*)((BYTE*)imnt + sizeof(IMAGE_NT_HEADERS64));
#endif

    if (mem)
    {
        result.sec0_VA = imsec[0].VirtualAddress + buffer;
        result.sec0_size = (imsec[0].Misc.VirtualSize / imnt->OptionalHeader.SectionAlignment + 1) * imnt->OptionalHeader.SectionAlignment;
    }
    else
    {
        result.sec0_VA = imsec[0].PointerToRawData + buffer;
        result.sec0_size = (imsec[0].SizeOfRawData / imnt->OptionalHeader.FileAlignment + 1) * imnt->OptionalHeader.FileAlignment;
    }

    return result;
}

char* GetProcNameByAddress(BYTE* mod_base, BYTE* addr)
{
    BYTE* mod_addr = 0;
    DWORD i;
    char* fname = 0;

    mod_addr = mod_base;

    IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)mod_addr;
    IMAGE_NT_HEADERS* imnth = (IMAGE_NT_HEADERS*)(mod_addr + imdos->e_lfanew);
    DWORD rva_exports = imnth->OptionalHeader.DataDirectory[0].VirtualAddress;

    IMAGE_EXPORT_DIRECTORY* imexp = (IMAGE_EXPORT_DIRECTORY*)(imnth->OptionalHeader.DataDirectory[0].VirtualAddress + mod_addr);
    DWORD AddressOfNames = imexp->AddressOfNames;
    size_t AddressOfNamesVA = (size_t)(AddressOfNames + mod_addr);
    DWORD real_index;
    BYTE* current_addr;

    for (i = 0; i < imexp->NumberOfNames; i++)
    {
        fname = (char*)(((DWORD*)AddressOfNamesVA)[i] + mod_addr);
        real_index = (size_t)((WORD*)(imexp->AddressOfNameOrdinals + mod_addr))[i];
        current_addr = (BYTE*)(((DWORD*)(imexp->AddressOfFunctions + mod_addr))[real_index] + mod_addr);
        if (current_addr == addr)
            break;
    }

    return fname;
}

int FindNtDLLHooks()
{
    char fpath[MAX_PATH] = { 0 };
    int diffs = 0;
    HMODULE ntdll = GetModuleHandle("ntdll.dll");
    GetModuleFileName(ntdll, fpath, MAX_PATH);
    HANDLE f = CreateFile(fpath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (f == INVALID_HANDLE_VALUE)
    {
        printf("cannot access [%s]\n", fpath);
        return 0;
    }
    DWORD size = GetFileSize(f, 0);
    DWORD read = 0;
    BYTE* buffer = (BYTE*)malloc(size);
    ReadFile(f, buffer, size, &read, 0);
    CloseHandle(f);

    MZPE_SPECS clean_dll = GetMZPESpecs(buffer, false);
    MZPE_SPECS bad_dll = GetMZPESpecs((BYTE*)ntdll, true);

    for (size_t i = 0; i < min(clean_dll.sec0_size, bad_dll.sec0_size); i++)
    {
        if ((clean_dll.sec0_VA[i] != bad_dll.sec0_VA[i]) && (bad_dll.sec0_VA[i] == 0xE9))
        {
            diffs++;
            char* pname = GetProcNameByAddress((BYTE*)ntdll, bad_dll.sec0_VA + i);
            printf("%p: clean_dll: [%02X %02X %02X %02X %02X] <--> bad_dll: [%02X %02X %02X %02X %02X] : {%s}\n", bad_dll.sec0_VA + i,
                clean_dll.sec0_VA[i + 0],
                clean_dll.sec0_VA[i + 1],
                clean_dll.sec0_VA[i + 2],
                clean_dll.sec0_VA[i + 3],
                clean_dll.sec0_VA[i + 4],
                bad_dll.sec0_VA[i + 0],
                bad_dll.sec0_VA[i + 1],
                bad_dll.sec0_VA[i + 2],
                bad_dll.sec0_VA[i + 3],
                bad_dll.sec0_VA[i + 4],
                pname);
        }
    }

    return diffs;
}

int main()
{
    int hooks = FindNtDLLHooks();
    if (!hooks)
        printf("PASSED");
    else
        printf("FAILED");
    return 0;
}
{% endraw %}