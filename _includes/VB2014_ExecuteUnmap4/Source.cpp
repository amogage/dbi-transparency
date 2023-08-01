{% raw %}
/*
ARCH: Windows x86
This example copies a piece of code to a memory region with intial RW acces then adds EXECUTE, RW and PAGE_GUARD and exception should be triggered
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
    VirtualProtect(buffer, 1, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &oldProt);

    bool pass = true;
    __try {
        _asm {
            call dword ptr [buffer]
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        if (GetExceptionCode() == EXCEPTION_GUARD_PAGE)
            pass = true;
    }

    if (!pass) {
        printf("FAILED");
        return 1;
    }

    printf("PASSED");
    return 0;
}



{% endraw %}