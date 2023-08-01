{% raw %}
/*
ARCH: Windows x86/x64
This example uses fxsave FPU instruction to save the FPU context and leak the EIP
DBIs most of the time do not correctly virtualize the FPU context
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
*/

#include <stdio.h>
#include <windows.h>
#include <intrin.h>

typedef DWORD* (*fptr)(size_t*,BYTE*);

int main()
{
    size_t _eip = 0, real_eip = 0;
    BYTE fxsave_memory[1024];
    DWORD* start_addr = 0;
    extern unsigned char eipfxsave_label[];

    memset(fxsave_memory, 0, sizeof(fxsave_memory));

#ifndef _M_X64
    _asm {
        call $ + 5                  
        add dword ptr[esp], 10
        pop _eip
        eipfxsave_label :
        fldpi
        lea ecx, [fxsave_memory]
        add ecx, 16
        and ecx, 0xFFFFFFF0
        fxsave [ecx]
        mov start_addr, ecx
    }
    real_eip = start_addr[2];
#else
    /*
    E8 00 00 00 00       call        $+5
    48 83 04 24 07       add         qword [rsp],7
    8F 01                pop         qword [rcx]
    D9 EB                fldpi
    CC                   int         3
    */
    BYTE code[29] = { 0xE8,0x00,0x00,0x00,0x00,0x48,0x83,0x04,0x24,0x07,0x8F,0x01,0xD9,0xEB,0x48,0x83,0xC2,0x10,0x48,0x83,0xE2,0xF0,0x0F,0xAE,0x02,0x48,0x89,0xD0,0xC3 };
    BYTE* target = (BYTE*)VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(target, code, sizeof(code));
    fptr myf = (fptr)target;
    start_addr = myf(&_eip, fxsave_memory);
    _eip &= 0xFFFFFFFF;
    real_eip = start_addr[2];
#endif

    printf("EIP:\t\t0x%X\nRealEIP:\t0x%X\n\n", _eip, real_eip);

    if (real_eip != _eip)
        printf("FAILED");
    else
        printf("PASSED");

    return 0;
}
{% endraw %}