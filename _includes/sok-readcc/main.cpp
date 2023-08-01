{% raw %}
/*
ARCH: Windows x86
This test executes a |FPU instruction at the start of a payload and store the FPU context using fnstenv instruction
Inside the memory region of the stored FPU context we should have the start EIP of the last FPU instruction
We compare the bytes of the allocated payload buffer with those found at the EIP inside the stored FPU context and they should match to pass
Credits to:
https://dl.acm.org/doi/abs/10.1145/3321705.3329819
https://github.com/season-lab/sok-dbi-security
*/

#include <windows.h>
#include <stdio.h>

#define FAIL            "FAILED"
#define SUCCESS         "PASSED"
#define PAGE_SIZE 4096

typedef DWORD(*fptr2)();

BOOL TEST_ReadFromCC() {
#ifndef _M_X64
    /*
    D9 EE                fldz
    9B                   wait
    D9 74 24 E4          fnstenv     [esp-1Ch]
    8B 44 24 F0          mov         eax,dword [esp-10h]
    C3                   ret
    */
    BYTE code[12] = { 0xD9,0xEE,0x9B,0xD9,0x74,0x24,0xE4,0x8B,0x44,0x24,0xF0,0xC3 };
#else
    /*
    D9 EE                fldz
    9B                   wait
    D9 74 24 E4          fnstenv     [rsp-1Ch]
    48 31 C0             xor         rax,rax
    8B 44 24 F0          mov         eax,dword [rsp-10h]
    C3                   ret
    */
    BYTE code[15] = { 0xD9,0xEE,0x9B,0xD9,0x74,0x24,0xE4,0x48,0x31,0xC0,0x8B,0x44,0x24,0xF0,0xC3 };
#endif
    printf("[*]Starting read from CC test.. (Hint: disable FPU protection)\n");

    DWORD a;
    BYTE* buffer = (BYTE*)VirtualAlloc(0, PAGE_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(buffer, code, sizeof(code));
    fptr2 myf = (fptr2)buffer;
    a = myf();

    __try {
        if (memcmp((void*)a, buffer, PAGE_SIZE))
        {
            VirtualFree(buffer, 0, MEM_RELEASE);
            return TRUE;
        }
    }
    __except (1) {//filter(GetExceptionCode(), GetExceptionInformation())) {
        VirtualFree(buffer, 0, MEM_RELEASE);
        return TRUE;
    }

    VirtualFree(buffer, 0, MEM_RELEASE);
    return FALSE;

}

int main(int argc, char * argv[]) 
{
    if (TEST_ReadFromCC())
        printf(FAIL);
    else
        printf(SUCCESS);

    return ERROR_SUCCESS;
}


{% endraw %}