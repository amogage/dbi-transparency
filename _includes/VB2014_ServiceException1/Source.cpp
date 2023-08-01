{% raw %}
/*
ARCH: Windows x86
This example fills 512 bytes above current stack pointer (ESP-512) with a specific pattern and then triggers a self modifying instruction
The DBI handling of self modifying instruction may force a backup of the DBI internal state in this stack region, considered to be FREE (return from potential functions using it)
If any byte in this 512 byte memory range gets changed, the test FAILS
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf
*/

#include <windows.h>
#include <stdio.h>
#define PATTERNSIZE 0x200

int main(int argc, CHAR* argv[])
{
    
    printf("Verifying that a pattern left on the stack is not smashed by SMC.");

    DWORD  oldProt;
    VirtualProtect(main, 8192, PAGE_EXECUTE_READWRITE, &oldProt);
    void* firstchangeaddr = NULL;
    unsigned char* bufferptr = (unsigned char*)VirtualAlloc(NULL, PATTERNSIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    _asm {       
        mov edi,esp
        sub edi, PATTERNSIZE
        mov ecx, PATTERNSIZE
        mov eax, 65        
        rep stosb        
        mov byte ptr smc, 0x90
smc:
        _emit 0xcc        
        mov esi, esp
        mov edi, bufferptr;
        sub esi, PATTERNSIZE
        mov ecx, PATTERNSIZE
        rep movsb

    }

    bool passed = true;
    int idx = 0;
    for (int i= PATTERNSIZE-1; i>=0; i--) {
        
        if ((idx++ % 32)==0 )
            printf("\n [ESP-%04x]  ", (PATTERNSIZE-1) - i);

        printf("%c", bufferptr[i] != 65 ? 'X' : '.');
        
        if (bufferptr[i] != 65) {
            
            if (!firstchangeaddr)
                firstchangeaddr = &bufferptr[i];

            passed = false;
        }
    }

    printf("\n");

    printf("> Address of first difference: %p\n", firstchangeaddr);

    VirtualFree(bufferptr, PATTERNSIZE, MEM_RELEASE);

    if (passed) {
        printf("PASSED");
        return 0;
    }

    printf("FAILED");
    return 1;
}


{% endraw %}