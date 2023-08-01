{% raw %}
/*
ARCH: Windows x86
This example transit from 32 bit to 64 bit to clear the process cmdline buffer in PEB
None of the existing DBIs are able to perform the instrumentation of this CPU transition, as they normally require both 32 and 64 bit dependencies to be loaded.
And there are other negative side-effects.
Not passing this test will result in execution crash.
Credits to "Joseph Sible-Reinstate Monica" : https://stackoverflow.com/questions/72674069/how-can-a-wow64-program-overwrite-its-command-line-arguments-as-seen-by-wmi
*/

#include <Windows.h>
#include <winternl.h>
#include <intrin.h>
#include <stdio.h>

BYTE payload[0x50] = {
    //32 bit code
    0x55,                                       //push ebp
    0x89,0xE5,                                  //mov ebp, esp
    0x83,0xE4,0xF0,                             //and esp, -0x10
    0x6A,0x33,                                  //push 0x33
    0xE8,0x0C,0x00,0x00,0x00,                   //call enter_x64
    0x83,0xEC,0x20,                             //sub esp, 0x20
    0xE8,0x09,0x00,0x00,0x00,                   //call continue_x64
    0x83,0xC4,0x20,                             //add esp, 0x20
    0xCB,                                       //retf
                                    //enter_x64:
    0xFF,0x1C,0x24,                             //call far dword [esp]
    0xC9,                                       //leave
    0xC3,                                       //ret
    //64 bit code
                                    //continue_x64:
    0x65,0x67,0x48,0xA1,0x30,0x00,0x00,0x00,    //mov rax, gs:[0x30]
    0x48,0x8B,0x40,0x60,                        //mov rax, [rax+0x60]
    0x48,0x8B,0x40,0x20,                        //mov rax, [rax+0x20]
    0x0F,0xB7,0x48,0x70,                        //mozx ecx, [rax+0x70]
    0x67,0xE3,0x0D,                             //jecxz finish
    0x57,                                       //push rdi
    0x48,0x8B,0x78,0x78,                        //mov rdi, [rax+0x78]
    0xD1,0xE9,                                  //shr ecx, 1
    0x31,0xC0,                                  //xor eax, eax
    0xF3,0x66,0xAB,                             //rep stosw
    0x5F,                                       //pop rdi
                                    //finish:
    0xC3,                                       //ret
    0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC    //align bytes (int3)
};

void ZeroCmdLine_32_32()
{ 
    PUNICODE_STRING CommandLine = &NtCurrentTeb()->ProcessEnvironmentBlock->ProcessParameters->CommandLine;
    if (USHORT Length = CommandLine->Length)
        __stosw((PUSHORT)CommandLine->Buffer, 0, Length / sizeof(WCHAR));
}

int main()
{
    BOOL bWow = FALSE;

    BYTE* reloc_payload = (BYTE*)VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(reloc_payload, payload, sizeof(payload));    
        
    ZeroCmdLine_32_32();

    if (IsWow64Process(GetCurrentProcess(), &bWow) && bWow)
        ((void (__fastcall *)())reloc_payload)();

    printf("PASSED");

    return 0;
}
{% endraw %}