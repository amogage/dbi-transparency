{% raw %}
/*
ARCH: Windows x86
Thsi example attempts to execute a piece of code with no EXECUTION rights, triggering an exception
When DBIs do not take kare on access rights, it is easy to execute this code and not trigger the exception, failing the test
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
*/

#include <Windows.h>
#include <stdio.h>

#define ASSEMBLY_SIZE 13
#define PAGE_SIZE 4096

typedef void (*fptr)();

/*
E8 00 00 00 00       call        $+5
50                   push        eax
B8 2A 00 00 00       mov         eax,2Ah
58                   pop         eax
C3                   ret
*/
static const unsigned char assembly[ASSEMBLY_SIZE] = { 0xE8, 0x00, 0x00, 0x00, 0x00, 0x50, 0xB8, 0x2A, 0x00, 0x00, 0x00, 0x58, 0xC3 };

int main()
{
    const int page_size = PAGE_SIZE;

    BYTE* to_exec = (BYTE*)VirtualAlloc(0, page_size, MEM_COMMIT, PAGE_READWRITE);
    if (to_exec == NULL)
        return 0;

    memcpy(to_exec, assembly, ASSEMBLY_SIZE);
    fptr myf = (fptr)to_exec;

    __try {
        myf();
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        printf("PASSED");
        VirtualFree(to_exec, 0, MEM_RELEASE);
        return 0;
    }

    VirtualFree(to_exec, 0, MEM_RELEASE);
    printf("FAILED");

    return 0;
}
{% endraw %}