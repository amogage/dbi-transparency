{% raw %}
/*
ARCH: Windows x86/x64
This example attempts to search for duplicates of a constant in memory regions with EXECUTE access rights.
The target memory where duplicates are found are most probable associated with DBI-cache
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
*/

#include <windows.h>
#include <stdio.h>

#pragma warning (disable:4996)

#ifndef _M_X64
#define MAX_MEM 0x7FFFFFFF
#else
#define MAX_MEM 0x7FFFFFFFFFFFFFFFULL
#endif

//enumerate memory regions
MEMORY_BASIC_INFORMATION* enumerate_memory(size_t& total_count)
{
    MEMORY_BASIC_INFORMATION crt = { 0 };
    MEMORY_BASIC_INFORMATION* regions = (MEMORY_BASIC_INFORMATION*)malloc(1000 * sizeof(MEMORY_BASIC_INFORMATION));
    memset(regions, 0, 1000 * sizeof(MEMORY_BASIC_INFORMATION));
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
        if ((crt.State & MEM_COMMIT) == MEM_COMMIT && ((crt.Protect == PAGE_EXECUTE_READ) || (crt.Protect == PAGE_EXECUTE_READWRITE)))
        {
            memcpy(&regions[i], &crt, sizeof(crt));
            i++;
        }
    }

    total_count = i;

    return regions;
}

int main()
{
    size_t total_count = 0;
    size_t i = 0;
    int times = 0;
    MEMORY_BASIC_INFORMATION fin = { 0 };
    //8 bytes crafted constant
    long long my_constant1 = 0xABCDEF0110FEDCBA;
    printf("Query mem ... ");
    //query memory regions
    auto memoryRegions = enumerate_memory(total_count);
    printf("[%d] ranges \n", total_count);

    printf("Search: ... \n");
    for (size_t i = 0; i < total_count; i++)
    {
        PBYTE addr = (PBYTE)memoryRegions[i].BaseAddress;
        PBYTE regionEnd = addr + memoryRegions[i].RegionSize;
        printf(".");
        while (addr < regionEnd - 10)
        {
            __try {
#ifndef _M_X64
                //if low DWORD match, show some byte-code
                if (*(DWORD*)addr == 0xABCDEF01)
                {
                    //search for high DWORD in next 20 bytes
                    for (int j = 0; j < 20; j++)
                    {
                        //if match, show bytes
                        if (*(DWORD*)(addr + j) == 0x10FEDCBA)
                        {
                            printf("\n%p: \n", addr);
                            printf("%02X", addr[0]);
                            for (int k = 1; k < 4; k++)
                                printf("_%02X", addr[k]);
                            for (int k = 4; k < j + 1; k++)
                                printf(" %02X", addr[k]);
                            for (int k = j + 1; k < j + 4; k++)
                                printf("_%02X", addr[k]);
                            for (int k = j + 4; k < 20; k++)
                                printf(" %02X", addr[k]);
                            printf("\n");
                            times++;
                            break;
                        }
                    }
                }
#else
                unsigned long long* x = (unsigned long long*)addr;
                if (x[0] == my_constant1)
                {
                    addr -= 2;
                    if (addr[0] && addr[1])
                    {
                        printf("%p: ", addr);
                        for (int j = 0; j < 14; j++)
                            printf("%02X", addr[j]);
                        printf("\n");
                        times++;
                    }
                    addr += 2;
                }
#endif
            }
            __except (EXCEPTION_EXECUTE_HANDLER) {
                printf("!");
                break;
            }
            addr++;
        }
    }
    free(memoryRegions);
    printf("\n");

    if (times == 1)
        printf("PASSED");
    else
        if (times > 1)
            printf("FAILED");

    return 0;
}

{% endraw %}