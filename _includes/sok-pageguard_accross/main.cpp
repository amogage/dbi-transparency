{% raw %}
/*
ARCH: Windows x86/x64
This test attempts to create a payload at the crossing of two 4K pages
While the start of the code right before the first page ends can execute, the code starting in the second page should trigger a specific exception
Not triggering this exception will lead to a failed test
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

BOOL TEST_PageGuardAcross() {

    printf("[*]Starting Page Guard across pages test..\n");

    BYTE* buffer;
#ifndef _M_X64
    /*
    B8 78 56 34 12       mov         eax, buffer_start
    C6 05 78 56 34 12 90 mov         byte [eax + exception_offset], 90h
                         exception_offset:
    CC                   int         3
                         buffer_start + 0x1000:
    90                   nop                         
    C3                   ret
    */
    BYTE code[15] = { 0xB8,0x78,0x56,0x34,0x12,0xC6,0x05,0x78,0x56,0x34,0x12,0x90,0xCC,0x90,0xC3 };
#else
    /*
    48 B8 00 00 8E 9D FA 01 00 00   mov         rax, buffer_start
    C6 80 FF 0F 00 00 90            mov         byte [rax+exception_offset],90h
                                    exception_offset:
    CC                              int         3
                                    buffer_start + 0x1000:
    90                              nop                                    
    C3                              ret
    */
    BYTE code[20] = { 0x48,0xB8,0x78,0x56,0x34,0x12,0x78,0x56,0x34,0x12,0xC6,0x80,0xFF,0x0F,0x00,0x00,0x90,0xCC,0x90,0xC3 };
#endif
    DWORD  oldProt;

    buffer = (BYTE*)VirtualAlloc(NULL, 2 * PAGE_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    memcpy(buffer + PAGE_SIZE - (sizeof(code) - 2), code, sizeof(code));
#ifndef _M_X64
    *(DWORD*)(buffer + PAGE_SIZE - (sizeof(code) - 2) + 1) = (DWORD)buffer;
    *(DWORD*)(buffer + PAGE_SIZE - (sizeof(code) - 2) + 7) = (DWORD)(buffer + PAGE_SIZE - 1);
#else
    *(DWORD64*)(buffer + PAGE_SIZE - (sizeof(code) - 2) + 2) = (DWORD64)buffer;
#endif

    VirtualProtect(buffer + PAGE_SIZE, 1, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &oldProt);
    fptr myf = (fptr)(buffer + PAGE_SIZE - (sizeof(code) - 2));
    __try {
        myf();
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        if (GetExceptionCode() == EXCEPTION_GUARD_PAGE) {
            VirtualFree(buffer, 0, MEM_RELEASE);
            return FALSE;
        }
    }

    VirtualFree(buffer, 0, MEM_RELEASE);
    return TRUE;
}

int main(int argc, char * argv[]) 
{
    if (TEST_PageGuardAcross())
        printf(FAIL);
    else
        printf(SUCCESS);

    return ERROR_SUCCESS;
}


{% endraw %}