{% raw %}
/*
ARCH: Windows x86
This example copies a piece of code to a memory region with intial RW acces then adds EXECUTE and no exception should trigger
On top of that, the memory region gets changed to NOACCESS and exception should be triggered this time
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf
*/

#include <windows.h>
#include <stdio.h>

char *buffer;

int main(int argc, CHAR* argv[])
{
    printf("Verifying that it is not possible to execute code from an unmapped page.\n");

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

    printf("* Test code placed in a PAGE_EXECUTE_READ page at %p.\n", buffer);

    bool pass = true;
    __try {
        printf("* Executing the test code (first round).\n");
        _asm {
            call dword ptr [buffer]
        }
        printf("* Test code executed.\n");
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        printf("* Got exception %x.\n", GetExceptionCode());
        pass = false;
    }

    if (!pass) {
        printf("FAILED");
        return 1;
    }

    printf("* Remapping the page at %p as PAGE_NOACCESS.\n", buffer);
    VirtualProtect(buffer, 1, PAGE_NOACCESS, &oldProt);

    pass = false;
    __try {
        printf("* Executing the test code (second round).\n");
        _asm {
            call dword ptr [buffer]
        }
        printf("* Test code executed.\n");
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        printf("* Got exception %x.\n", GetExceptionCode());
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