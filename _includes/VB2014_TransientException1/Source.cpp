{% raw %}
/*
ARCH: Windows x86
This example allocates 2 pages with EXECUTE_READ_WRITE access and moves a few instruction at the intersection of these pages
The code is actually sligthly crossing the boundaries of the first page
The second page gets additionally PAGE_GUARD access, such that when code gets executed, the first instruction in the second page should trigger an EXCEPTION_GUARD_PAGE exception
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf
*/

#include <windows.h>
#include <stdio.h>

#define PAGE_SIZE 4096

char *buffer;

int main(int argc, CHAR* argv[])
{
    printf("Verifying that a PAGE_GUARD protection is not lost due to SMC.\n");

    buffer = (char *) VirtualAlloc(NULL, 2 * PAGE_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    
    printf("* Allocated two PAGE_EXECUTE_READWRITE pages at %p.\n", buffer);

    uintptr_t Start, PageBreak, End;
    
    _asm {
            mov Start, offset start
            mov PageBreak, offset pb
            mov End, offset end
            jmp end
    }

    _asm {
start:
            mov eax, buffer
            mov byte ptr [eax + PAGE_SIZE - 1], 0x90
            nop
pb:
            nop
            ret
end:
    }

    uintptr_t dest = (uintptr_t) buffer + PAGE_SIZE - (PageBreak - Start);

    printf("* Placing test code around the page boundary %p - %p.\n", (void *) dest, (void *) (dest + (End - Start)));

    memcpy((void *) dest, (void *) Start, End - Start);

    DWORD  oldProt;
    VirtualProtect(buffer + PAGE_SIZE, 1, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &oldProt);

    printf("* Adding PAGE_GUARD protection to page %p.\n", (void *) (buffer + PAGE_SIZE));

    __try {
        printf("* Executing the test code.\n");
        _asm {
            call dword ptr [dest]
        }
        printf("* Test code executed.\n");
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        printf("* Got exception %x.\n", GetExceptionCode());
        if (GetExceptionCode() == EXCEPTION_GUARD_PAGE) {
            printf("PASSED");
            return 0;
        }
    }

    printf("FAILED");
    return 1;
}


{% endraw %}