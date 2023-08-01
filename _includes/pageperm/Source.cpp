{% raw %}
/*
ARCH: Windows x86
This example counts how many memory chunks exist with RWE poermission rights
There should be none to pass the test, otherwise these are some kind of paylaods, specific to DBIs but also to other kind of code injection
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
*/

#include <windows.h>
#include <stdio.h>

int enumerate_rwx_memory()
{
    MEMORY_BASIC_INFORMATION crt = { 0 };

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
        if (((crt.State & MEM_COMMIT) == MEM_COMMIT) && (crt.Protect == PAGE_EXECUTE_READWRITE))
            i++;
    }

    return i;
}

int main()
{
    int rwx = enumerate_rwx_memory();
    printf("rwx = %d\n", rwx);
    if (!rwx)
        printf("PASSED");
    else
        printf("FAILED");
    return 0;
}


{% endraw %}