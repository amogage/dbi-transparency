{% raw %}
/*
ARCH: Windows x86/x64
This test leaks EIP from the FPU context, using fnstenv instruction to store the FPU context in a specific memory region
Credits to:
https://dl.acm.org/doi/abs/10.1145/3321705.3329819
https://github.com/season-lab/sok-dbi-security
*/

#include <windows.h>
#include <stdio.h>

#define FAIL            "FAILED"
#define SUCCESS         "PASSED"
#define PAGE_SIZE 4096

typedef DWORD(*fptr3)(DWORD&);

BOOL TEST_FPU() {
#ifndef _M_X64
    /*
    D9 EE                           fldz
    9B                              wait
    D9 74 24 E4                     fnstenv     [esp-1Ch]
    8B 44 24 04                     mov         eax,dword [esp+4]
    C7 00 78 56 34 12               mov         dword [eax],12345678h
    8B 44 24 F0                     mov         eax,dword [esp-10h]
    C3                              ret
    */
    BYTE code[22] = { 0xD9,0xEE,0x9B,0xD9,0x74,0x24,0xE4,0x8B,0x44,0x24,0x04,0xC7,0x00,0x78,0x56,0x34,0x12,0x8B,0x44,0x24,0xF0,0xC3 };
#else
    /*
    D9 EE                           fldz
    9B                              wait
    D9 74 24 E4                     fnstenv     [rsp-1Ch]
    48 31 C0                        xor         rax,rax
    8B 44 24 F0                     mov         eax,dword [rsp-10h]
    48 BB 78 56 34 12 78 56 34 12   mov         rbx,1234567812345678h
    48 C1 E3 20                     shl         rbx,20h
    48 C1 EB 20                     shr         rbx,20h
    48 89 19                        mov         qword [rcx],rbx
    C3                              ret
    */
    BYTE code[36] = { 0xD9, 0xEE, 0x9B, 0xD9, 0x74, 0x24, 0xE4, 0x48, 0x31, 0xC0, 0x8B, 0x44, 0x24, 0xF0, 0x48, 0xBB, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12, 0x48, 0xC1, 0xE3, 0x20, 0x48, 0xC1, 0xEB, 0x20, 0x48, 0x89, 0x19, 0xC3 };                 
#endif
    printf("[*]Starting FPU test..\n");

    DWORD a, b;
    BYTE* buffer = (BYTE*)VirtualAlloc(0, PAGE_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(buffer, code, sizeof(code));
#ifndef _M_X64
    *(DWORD*)(buffer + 13) = (DWORD)buffer;
#else
    * (DWORD64*)(buffer + 16) = (DWORD64)buffer;
#endif
    fptr3 myf = (fptr3)buffer;
    a = myf(b);
    VirtualFree(buffer, 0, MEM_RELEASE);

    return (a != b) ? TRUE : FALSE;
}

int main(int argc, char * argv[]) 
{
    if (TEST_FPU())
        printf(FAIL);
    else
        printf(SUCCESS);

    return ERROR_SUCCESS;
}


{% endraw %}