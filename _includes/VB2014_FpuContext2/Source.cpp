{% raw %}
/*
ARCH: Windows x86
This example uses the 16 bit version of fnstenv to leak the DBI-cache low-WORD EIP, of the last FPU instruction
The low-word EIP found in the FPU context should match the low-word of the actual instruction EIP
Credits to:
https://github.com/sf2team/vb2014
https://www.virusbulletin.com/uploads/pdf/conference_slides/2014/sponsorAVAST-VB2014.pdf
*/

#include <windows.h>
#include <stdio.h>

WORD fpu_save_area[7];

int main(int argc, CHAR* argv[])
{
    printf("Verifying that the IP leaked by 0x66 FNSTENV is properly virtualized.\n");

    unsigned short Start;

    _asm {
start:
        fsin
        _emit 0x66
        fnstenv fpu_save_area
        lea eax, start
        mov word ptr Start, ax
    }

    printf("* Guest IP: %x\n", Start);
    printf("* Leaked IP: %x\n", fpu_save_area[3]);

    if (fpu_save_area[3] == Start) {
        printf("PASSED");
        return 0;
    }

    printf("FAILED");
    return 1;
}

{% endraw %}