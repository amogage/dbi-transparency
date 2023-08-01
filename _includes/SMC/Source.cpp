{% raw %}
/*
ARCH: Windows x86/x64
This example tests the possibility of the DBI to instrment self-modifying code
If code is not correct instrumented, the test fails
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
*/

#include <Windows.h>
#include <stdio.h>

typedef void (*fptr)(size_t*);

int main()
{
    size_t change_me = 2;
    DWORD oldp;
    int page_size = 0x1000;
    BYTE* page_start = (BYTE*)&main - (size_t)&main % page_size;    

#ifndef _M_X64
    VirtualProtect(page_start, page_size, PAGE_EXECUTE_READWRITE, &oldp);
    _asm {
        call $+5
        pop ecx
        mov byte ptr[ecx + 6], 0
        mov eax, 1
        mov change_me, eax
    }
    VirtualProtect(page_start, page_size, oldp, &oldp);
#else
    /*
    E8 00 00 00 00       call        $+5
    5A                   pop         rdx
    C6 42 08 00          mov         byte [rdx + 8], 0
    48 C7 C0 01 00 00 00 mov         rax, 1
    48 89 01             mov         qword [rcx], rax
    C3                   ret
    */
    BYTE code[21] = { 0xE8,0x00,0x00,0x00,0x00,0x5A,0xC6,0x42,0x08,0x00,0x48,0xC7,0xC0,0x01,0x00,0x00,0x00,0x48,0x89,0x01,0xC3 };
    BYTE* target = (BYTE*)VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(target, code, sizeof(code));
    fptr myf = (fptr)target;
    myf(&change_me);
#endif    

    printf("change_me\t%d\n", change_me);
    if (!change_me)
        printf("PASSED");
    else
        printf("FAILED");
    return 0;
}
{% endraw %}