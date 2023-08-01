{% raw %}
/*
ARCH: Windows x86/x64
This example frees memory that is not known: it was not expected on native execution and maybe comes form injected code
Usually when instrumented, the execution crashes
*/

#include <Windows.h>
#include <stdio.h>
#include <intrin.h>

#pragma warning (disable:4996)

#ifndef _M_X64
#define MAX_MEM 0x7FFFFFFF
#else
#define MAX_MEM 0x7FFFFFFFFFFFFFFFULL
#endif

struct MEM_INFO {
    BYTE* base;
    DWORD size;
    DWORD attr;
    char label[20];
};

//enumerate mmeory regions
MEM_INFO* enumerate_memory(size_t* total_count)
{
    MEMORY_BASIC_INFORMATION crt = { 0 };
    MEM_INFO* regions = (MEM_INFO*)malloc(1000 * sizeof(MEM_INFO));
    memset(regions, 0, 1000 * sizeof(MEM_INFO));
    const PBYTE MaxAddress = (PBYTE)MAX_MEM;
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
        if ((crt.State & MEM_COMMIT) == MEM_COMMIT && ((crt.Protect == PAGE_EXECUTE_READ) || (crt.Protect == PAGE_EXECUTE_READWRITE) || (crt.Protect == PAGE_READWRITE) || (crt.Protect == PAGE_READONLY)))
        {
            regions[i].base = (BYTE*)crt.BaseAddress;
            regions[i].size = crt.RegionSize;
            regions[i].attr = crt.Protect;
            memset(regions[i].label, 0, 20);
            i++;
        }
    }

    int count = 0;
    for (int j = 1; j < i; j++)
    {
        if ((regions[j].attr == PAGE_EXECUTE_READ) && (regions[j - 1].attr == PAGE_READONLY))
        {
            count++;
            if (count == 2)
            {
                i = j - 1;
                break;
            }
        }
    }

    total_count[0] = i;

    return regions;
}

//label specific ranges
void LabelSpecific(MEM_INFO* regions, size_t total, BYTE* base, const char* label)
{
    while (true)
    {
        __try {
            char x = base[0];
            base--;
        }
        __except (1) {
            base++;
            break;
        }
    }
    BYTE* last = base;
    while (true)
    {
        __try {
            char x = last[0];
            last++;
        }
        __except (1) {
            break;
        }
    }
    for (int i = 0; i < total; i++)
    {
        if ((regions[i].base >= base) && (regions[i].base < last))
            strcpy(regions[i].label, label);
    }
}

//Label generic ranges
void LabelRegions(MEM_INFO* regions, size_t total)
{
    char test[10] = { 0 };
    BYTE empty[1024] = { 0 };

    //main module
    BYTE* hm = (BYTE*)GetModuleHandle(0);
    IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)hm;
    IMAGE_NT_HEADERS* imnt = (IMAGE_NT_HEADERS*)(hm + imdos->e_lfanew);
    DWORD size = imnt->OptionalHeader.SizeOfImage;
    for (int i = 0; i < total; i++)
    {
        if ((regions[i].base >= hm) && (regions[i].base < hm + size))
            strcpy(regions[i].label, "main-module");
        if (!strcmp((char*)regions[i].base, "Actx "))
            strcpy(regions[i].label, "Actx");
        if (!memcmp(regions[i].base, empty, sizeof(empty)) && !memcmp(regions[i].base + regions[i].size - sizeof(empty), empty, sizeof(empty)))
            strcpy(regions[i].label, "empty");        
    }

    //stack
    LabelSpecific(regions, total, (BYTE*)test, "stack-main-thread");
    LabelSpecific(regions, total, (BYTE*)regions, "heap-main-thread");
#ifndef _M_X64
    LabelSpecific(regions, total, (BYTE*)__readfsdword(0x30), "PEB");
#else
    LabelSpecific(regions, total, (BYTE*)__readgsqword(0x60), "PEB");
#endif
    LabelSpecific(regions, total, (BYTE*)GetEnvironmentStrings(), "environment");    

    for (int i = 0; i < total; i++)
    {
        if (!regions[i].label[0])
        {
            if ((regions[i].attr != PAGE_EXECUTE_READ) && (regions[i].attr != PAGE_EXECUTE_READWRITE))
            {
                size_t* traverse = (size_t*)regions[i].base;
                DWORD total_local = 0;
                for (int k = 0; k < regions[i].size / sizeof(size_t); k++)
                {
                    if ((traverse[k] & -65536) == ((DWORD)regions[i].base & -65536))
                        total_local++;
                }
                if (total_local)
                    sprintf(regions[i].label, "other-stack");
                else
                    sprintf(regions[i].label, "other-heap");
            }
            else
                sprintf(regions[i].label, "UNKNOWN");
        }
    }
}

int main()
{
    size_t total = 0;

    MEM_INFO* regions = enumerate_memory(&total);
    LabelRegions(regions, total);
    
    printf("total: %d\n", total);    
    for (int i = 0; i < total; i++)
        printf("%d. %p - %X, %X [%s]\n", i + 1, regions[i].base, regions[i].size, regions[i].attr, regions[i].label);

    printf("Free Unknown regions ... ");
    for (int i = 0; i < total; i++)
    {
        if (!strcmp(regions[i].label, "UNKNOWN"))
        {
            __try {
                free(regions[i].base);
                printf("[free]\n");
            }
            __except (1) {
                __try {
                    VirtualFree(regions[i].base, 0, MEM_RELEASE);
                    printf("[VirtualFree]\n");
                }
                __except(1) {
                    __try {
                        LocalFree(regions[i].base);
                        printf("[LocalFree]\n");
                    }
                    __except (1) {
                        printf("FAILED");
                    }
                }
            }
        }
    }

    printf("\n\nPASSED"); 

	return 0;
}


{% endraw %}