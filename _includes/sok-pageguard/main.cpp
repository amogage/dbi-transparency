{% raw %}
/*
ARCH: Windows x86/x64
This test attempts to execute a piece of code having PAGE_GUARD access rights, triggering a specific exception.
Not triggering the exception leads to failing the test
Credits to:
https://dl.acm.org/doi/abs/10.1145/3321705.3329819
https://github.com/season-lab/sok-dbi-security
 */

#include <windows.h>
#include <stdio.h>

#define FAIL            "FAILED"
#define SUCCESS         "PASSED"
#define PAGE_SIZE 4096

typedef void (*fptr)();

BOOL TEST_PageGuard() {

    printf("[*]Starting Page Guard test..\n");

    UCHAR* pMem = NULL;
    SYSTEM_INFO SystemInfo = { 0 };
    DWORD OldProtect = 0;
    UCHAR* pAllocation = NULL;

    // Retrieves information about the current system.
    GetSystemInfo(&SystemInfo);

    // Allocate memory 
    pAllocation = (UCHAR*)VirtualAlloc(NULL, SystemInfo.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (pAllocation == NULL) {
        printf("Failed VirtualAlloc\n");
        return FALSE;
    }

    //C3        ret
    pAllocation[0] = 0xc3;

    // Make the page a guard page         
    if (VirtualProtect(pAllocation, SystemInfo.dwPageSize, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &OldProtect) == 0) {
        printf("Failed VirtualProtect\n");
        return FALSE;
    }

    __try
    {
        ((fptr)pAllocation)(); // Exception or execution, which shall it be :D?
    }
    __except (1)//filter(GetExceptionCode(), GetExceptionInformation()))
    {
        VirtualFree(pAllocation, NULL, MEM_RELEASE);
        return FALSE;
    }

    VirtualFree(pAllocation, NULL, MEM_RELEASE);
    return TRUE;
}

int main(int argc, char * argv[]) 
{
    if (TEST_PageGuard())
        printf(FAIL);
    else
        printf(SUCCESS);

    return ERROR_SUCCESS;
}


{% endraw %}