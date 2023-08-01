{% raw %}
/*
ARCH: Windows x86
This example allocates a RW payload and adds EXECUTE rights so no exception should trigger, then switches to ReadOnly and exception should trigger
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf
*/

#include <windows.h>
#include <stdio.h>

char *buffer;

int main(int argc, CHAR* argv[])
{
    buffer = (char *) VirtualAlloc(NULL, 1, MEM_COMMIT, PAGE_READWRITE);
    uintptr_t Start, End;

    _asm {
            mov Start, offset start
            mov End, offset end
            jmp end
    }

    _asm {
start:
        nop
        ret
end:
    }

    memcpy(buffer, (void *) Start, End - Start);

    DWORD  oldProt;
    VirtualProtect(buffer, 1, PAGE_EXECUTE_READ, &oldProt);

    bool pass = true;
    __try {
        _asm {
            call dword ptr [buffer]
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        pass = false;
    }

    if (!pass) {
        printf("FAILED");
        return 1;
    }

    VirtualProtect(buffer, 1, PAGE_READONLY, &oldProt);

    pass = false;
    __try {
        _asm {
            call dword ptr [buffer]
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        if (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION)
            pass = true;
    }

    if (!pass) {
        printf("FAILED");
        return 2;
    }

    printf("PASSED");
    return 0;
}



{% endraw %}