{% raw %}
/*
ARCH: Windows x86
This examnple maps potential known DLLs as part of different DBIs
If any of them is found, the test fails
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
*/

#include <Windows.h>
#include <stdio.h>
#include <psapi.h>

#pragma warning (disable:4996)

char modules[26][30] = {
    "pinvm.dll",
    "pincrt.dll",
    "pindemonium.dll",
    "proctracer.dll",
    "pinvmshield.dll",
    "sok-mitigations.dll",
    "qbdi.dll",
    "flow.dll",
    "flowuser.dll",
    "hook.dll",
    "injector.dll",
    "instructioncounter.dll",
    "drmgr.dll",
    "drutil.dll",
    "dynamorio.dll",
    "unpacker.dll",
    "apicontrol.dll",
    "cfg.dll",
    "dbi.dll",
    "disasm.dll",
    "exceptionhandler.dll",
    "instrcontrol.dll",
    "logger.dll",
    "shield.dll",
    "taintanalysis.dll",
    "translator.dll"
};

HMODULE* enumerate_memory(size_t& total_count)
{
    MEMORY_BASIC_INFORMATION crt = { 0 };
    HMODULE* regions = (HMODULE*)malloc(1000 * sizeof(HMODULE));
    memset(regions, 0, 1000 * sizeof(HMODULE));

#ifndef _M_X64
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
        if ((crt.State & MEM_COMMIT) == MEM_COMMIT && ((crt.Protect == PAGE_READONLY) || (crt.Protect == PAGE_READWRITE) || (crt.Protect == PAGE_EXECUTE_READ) || (crt.Protect == PAGE_EXECUTE_READWRITE) || (crt.Protect == PAGE_EXECUTE_WRITECOPY)))
        {
            if (*(WORD*)crt.BaseAddress == 'ZM')
            {
                IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)crt.BaseAddress;
                if (imdos->e_lfanew < 0x1000)
                {
                    IMAGE_NT_HEADERS* imnt = (IMAGE_NT_HEADERS*)((BYTE*)crt.BaseAddress + imdos->e_lfanew);
                    if (imnt->FileHeader.Characteristics & IMAGE_FILE_DLL)
                        regions[i++] = (HMODULE)crt.BaseAddress;
                }
            }
        }
    }

    total_count = i;

    return regions;
}

char* GetModName(HMODULE mod_addr)
{
    IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)mod_addr;
    IMAGE_NT_HEADERS* imnth = (IMAGE_NT_HEADERS*)((BYTE*)mod_addr + imdos->e_lfanew);
    if (imnth->FileHeader.Machine == 0x14C)
    {
        DWORD rva_exports = imnth->OptionalHeader.DataDirectory[0].VirtualAddress;
        DWORD rva_name = ((DWORD*)((BYTE*)mod_addr + rva_exports))[3];
        return (char*)((BYTE*)mod_addr + rva_name);
    }
    
    IMAGE_NT_HEADERS64* imnth2 = (IMAGE_NT_HEADERS64*)((BYTE*)mod_addr + imdos->e_lfanew);
    DWORD rva_exports = imnth2->OptionalHeader.DataDirectory[0].VirtualAddress;
    DWORD rva_name = ((DWORD*)((BYTE*)mod_addr + rva_exports))[3];
    return (char*)((BYTE*)mod_addr + rva_name);
}

void transform(char* data)
{
    for (int i = 0; i < strlen(data); i++)
        if (data[i] >= 'A' && data[i] <= 'Z')
            data[i] |= 0x20;
}

int main()
{
    HMODULE hMods[1024];
    HANDLE hProcess = GetCurrentProcess();
    DWORD cbNeeded;
    unsigned int i;
    size_t total = 0;
    int detected = 0;

    HMODULE* all_mods = enumerate_memory(total);
    printf("total modules: %d\n", total);

    for (i = 0; i < total; i++)
    {
        char crt[100];
        char* modname = GetModName(all_mods[i]);
        strcpy(crt, modname);
        transform(crt);
        printf("[%s]\n", crt);

        for (int j = 0; j < 26; j++)
        {
            if (!strcmp(crt, modules[j]))
                detected++;
        }
    }

    if (!detected)
        printf("PASSED");
    else
        printf("FAILED");

	return 0;
}
{% endraw %}