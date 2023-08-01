{% raw %}
/*
ARCH: Windows x86
This example search for references to a constant string, leading to DBI-cache
If DBI-cache refs are found, code is shifted, such that triggering additional executions of the modified code will eventually execute a carefully crafted payload to exit instrumentation
Strings "This code is not instrumented" should not be displayed, if everything works ok
*/

#include <windows.h>
#include <stdio.h>
#include <map>
#include <set>
using namespace std;

#pragma warning (disable:4996)

#ifndef _M_X64
#define MAX_MEM 0x7FFFFFFF
#else
#define MAX_MEM 0x7FFFFFFFFFFFFFFFULL
#endif

#define INSTR_RET 0xC3

char my_trap_string[16] = "A trap for DBI";
BYTE* real_location = 0;
BYTE backup_byte;

void cached_func();

set<BYTE*> FindStringInDBICache()
{
    MEMORY_BASIC_INFORMATION crt = { 0 };
    MEMORY_BASIC_INFORMATION crt1 = { 0 };
    BYTE* region = 0;
    map<BYTE*,DWORD> unique;
    set<BYTE*> found;    
    const PBYTE MaxAddress = (PBYTE)MAX_MEM;
    PBYTE addr = 0;

    VirtualQuery(my_trap_string, &crt1, sizeof(MEMORY_BASIC_INFORMATION));
    printf("INITIAL-STR: %p:%p\n", crt1.BaseAddress, my_trap_string);
    VirtualQuery(cached_func, &crt1, sizeof(MEMORY_BASIC_INFORMATION));
    //printf("INITIAL-CODE: %p:%p:%x %08X\n", crt1.BaseAddress, (BYTE*)main + 3, *(BYTE*)((BYTE*)main + 3), *(DWORD*)((BYTE*)main + 4));

    while (addr < MaxAddress)
    {
        memset(&crt, 0, sizeof(crt));
        if (VirtualQuery(addr, &crt, sizeof(MEMORY_BASIC_INFORMATION)) <= 0)
            break;
        addr += crt.RegionSize;
        if (crt.State == MEM_FREE)
            continue;
        if ((crt.State & MEM_COMMIT) == MEM_COMMIT && ((crt.Protect == PAGE_EXECUTE_READ) || (crt.Protect == PAGE_EXECUTE_READWRITE)))
        {
            if ((BYTE*)crt.BaseAddress != (BYTE*)crt1.BaseAddress)
            {                
                for (size_t i = 0; i < crt.RegionSize - 8; i++)
                {
                    //search for string ptr duplicates
#ifndef _M_X64
                    if (*(DWORD*)((BYTE*)crt.BaseAddress + i) == (DWORD)my_trap_string)
                    {
                        printf("CURENT: %p:%x:%p:%x %x\n", crt.BaseAddress, (BYTE*)crt.BaseAddress + i, crt.Protect, *(BYTE*)((BYTE*)crt.BaseAddress + i - 1), *(DWORD*)((BYTE*)crt.BaseAddress + i));
                        unique[(BYTE*)crt.BaseAddress] = crt.RegionSize;
                        found.insert((BYTE*)crt.BaseAddress + i);
                    }
#else
                    //lea rdx, [mem-relative]
                    size_t str_va = *(DWORD*)((BYTE*)crt.BaseAddress + i) + (size_t)((BYTE*)crt.BaseAddress + i) + 4;
                    if (str_va == (size_t)my_trap_string)
                    {
                        printf("CURENT: %p:%x:%p:%x %x\n", crt.BaseAddress, (BYTE*)crt.BaseAddress + i, crt.Protect, *(BYTE*)((BYTE*)crt.BaseAddress + i - 3), *(DWORD*)((BYTE*)crt.BaseAddress + i));
                        unique[(BYTE*)crt.BaseAddress] = crt.RegionSize;
                        found.insert((BYTE*)crt.BaseAddress + i);
                    }
#endif
                }
            }
        }
    }

    return found;
}

void execute_outside_dbi()
{
    printf("0. This code is not instrumented!!!\n");
    printf("1. This code is not instrumented!!!\n");
    printf("2. This code is not instrumented!!!\n");
    printf("3. This code is not instrumented!!!\n");
    printf("4. This code is not instrumented!!!\n");
    printf("5. This code is not instrumented!!!\n");
    printf("6. This code is not instrumented!!!\n");
    printf("7. This code is not instrumented!!!\n");
    printf("8. This code is not instrumented!!!\n");
    printf("9. This code is not instrumented!!!\n");
    *(BYTE*)(real_location + 4) = backup_byte;

#ifndef _M_X64
    _asm {
        mov ecx, real_location
        add ecx, 4
        jmp ecx
    }
#else
    //to do
#endif
}

void cached_func()
{
    printf("\n[%s]\n\n", my_trap_string);
}

int main()
{
    auto f1 = FindStringInDBICache();
    //force instrumentation to move to DBI-cache the ref to our string
    cached_func();
    //find hardcoded string address in DBI cache
    auto f2 = FindStringInDBICache();    

    //diferenta intre ce am gasit, doar una din locatii este cea corecta
    for (auto it : f2)
    {
        if (f1.find(it) == f1.end())
        {
            real_location = it;
            printf("CACHE location: %p\n", real_location);
            printf("\nFAILED");
        }
    }

    if (real_location)
    {
        //edit code in DBI-cache
        backup_byte = *(BYTE*)(real_location + 4);
        *(DWORD*)real_location = (DWORD)execute_outside_dbi;
        *(BYTE*)(real_location + 4) = INSTR_RET;
        cached_func();
    }
    else
        printf("\nPASSED");

    return 0;
}
{% endraw %}