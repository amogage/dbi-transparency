{% raw %}
/*
ARCH: Windows x86
This example attempts to execute a RW ret opcode, and it should trigger the exception to pass
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf
*/

#include <windows.h>
#include <stdio.h>

int main(int argc, CHAR* argv[])
{
    char *buffer = (char *) VirtualAlloc(NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    //C3  RET
    *buffer = 0xc3;

    __try {
        _asm call buffer
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        if (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) {
            printf("PASSED");
            return 0;
        }
    }

    printf("FAILED");
    return 1;
}


{% endraw %}