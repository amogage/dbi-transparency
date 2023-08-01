{% raw %}
/*
ARCH: Windows x86/x64
This example use a SEH exception filter to leak te DBI-cache EIP comparing the exception context EIP with FPU-context EIP 
While te EIP is fixed upon handler execution, DBI doesn't seem to properly virtualize it inside the FPU context
On a native execution, they should match
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
*/

#include <windows.h>
#include <stdio.h>

size_t real_eip1 = 0;
size_t real_eip2 = 0;

typedef void (*fptr)(size_t*);

//filter function - extract EIP from exception context as well as EIP from the FPU state
int filter(DWORD code, struct _EXCEPTION_POINTERS *ep)
{
#ifndef _M_X64
    //recover FPU-saved EIP
    real_eip1 = ep->ContextRecord->FloatSave.ErrorOffset;
    //recover exception EIP and adjust it to match the last FPU instruction EIP
    real_eip2 = ep->ContextRecord->Eip - 2;
#else
    real_eip1 = ep->ContextRecord->FltSave.ErrorOffset;
    real_eip2 = (ep->ContextRecord->Rip - 2) & 0xFFFFFFFF;
#endif
    return EXCEPTION_EXECUTE_HANDLER;
}

int main()
{
    size_t _eip = 0;    

    __try {
#ifndef _M_X64
        _asm {
            call $+5
            add dword ptr [esp], 7
            pop _eip
            fldpi                       //using any FPU instruction, force a copy of the EIP inside the FPU context
            _emit 0xCC                  //trigger int3
        }
#else
        /*        
        E8 00 00 00 00       call        $+5  
        48 83 04 24 07       add         qword [rsp],7  
        8F 01                pop         qword [rcx]  
        D9 EB                fldpi  
        CC                   int         3  
        */
        BYTE code[15] = { 0xE8,0x00,0x00,0x00,0x00,0x48,0x83,0x04,0x24,0x07,0x8F,0x01,0xD9,0xEB,0xCC };
        BYTE* target = (BYTE*)VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        memcpy(target, code, sizeof(code));
        fptr myf = (fptr)target;
        myf(&_eip);        
#endif
    }
    __except (filter(GetExceptionCode(), GetExceptionInformation())) {
        _eip &= 0xFFFFFFFF;
        //the 2 EIPs must match
        printf("EIP:\t\t0x%X\nRealEIP1:\t0x%X\nRealEIP2:\t0x%X\n\n", _eip, real_eip1, real_eip2);
        if (real_eip1 != _eip || real_eip2 != _eip || real_eip1 != real_eip2)
            printf("FAILED");
        else
            printf("PASSED");
        return 0;
    }

    printf("FAILED");
    return 0;
}
{% endraw %}