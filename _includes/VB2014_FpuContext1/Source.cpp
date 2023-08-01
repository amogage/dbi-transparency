{% raw %}
/*
ARCH: Windows x86
This example uses fnstenv to leak the DBI-cache EIP, of the last FPU instruction
The EIP in the FPU context should match the buffer where the instruction was executed in the first place
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf
*/

#include <windows.h>
#include <stdio.h>

DWORD fpu_save_area[7];

int main(int argc, CHAR* argv[])
{
    printf("Verifying that the EIP leaked by FNSTENV is properly virtualized.\n");

    unsigned int Start;

    _asm {
Start:
        fsin
        fnstenv fpu_save_area
        lea eax, start
        mov Start, eax
    }

    printf("* Guest EIP: %x\n", Start);
    printf("* Leaked EIP: %x\n", fpu_save_area[3]);

    if (fpu_save_area[3] == Start) {
        printf("PASSED");
        return 0;
    }

    printf("FAILED");
    return 1;
}


{% endraw %}