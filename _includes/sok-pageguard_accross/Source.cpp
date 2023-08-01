{% raw %}
/*
This is a set of 4 examples implemented by SoK:
 - usage of pageguard accross 2 memory regions
*/

#include <windows.h>
#include <stdio.h>

#define FAIL            "FAILED\n"
#define SUCCESS         "PASSED\n"
#define PAGE_SIZE 4096

BOOL TEST_PageGuard();
BOOL TEST_PageGuardAcross();
BOOL TEST_ReadFromCC();
BOOL TEST_FPU();

typedef void (*fptr)();
typedef DWORD(*fptr2)();
typedef DWORD(*fptr3)(DWORD&);

BOOL TEST_PageGuard() {

    printf("[*]Starting Page Guard test..\n");

    UCHAR* pMem = NULL;
    SYSTEM_INFO SystemInfo = { 0 };
    DWORD OldProtect = 0;
    UCHAR* pAllocation = NULL;

    // Retrieves information about the current system.
    GetSystemInfo(&SystemInfo);

    // Allocate memory 
    pAllocation = (UCHAR*)VirtualAlloc(NULL, SystemInfo.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (pAllocation == NULL) {
        printf("Failed VirtualAlloc\n");
        return FALSE;
    }

    pAllocation[0] = 0xc3;

    // Make the page a guard page         
    if (VirtualProtect(pAllocation, SystemInfo.dwPageSize, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &OldProtect) == 0) {
        printf("Failed VirtualProtect\n");
        return FALSE;
    }

    __try
    {
        ((void(*)())pAllocation)(); // Exception or execution, which shall it be :D?
    }
    __except (1)//filter(GetExceptionCode(), GetExceptionInformation()))
    {
        VirtualFree(pAllocation, NULL, MEM_RELEASE);
        return FALSE;
    }

    VirtualFree(pAllocation, NULL, MEM_RELEASE);
    return TRUE;
}

BOOL TEST_PageGuardAcross() {

    printf("[*]Starting Page Guard across pages test..\n");

    BYTE* buffer;
    //UINT32 Start, PageBreak, End, dest;
#ifndef _M_X64
    BYTE code[15] = { 0xB8,0x78,0x56,0x34,0x12,0xC6,0x05,0x78,0x56,0x34,0x12,0x90,0xCC,0x90,0xC3 };
#else
    BYTE code[20] = { 0x48,0xB8,0x78,0x56,0x34,0x12,0x78,0x56,0x34,0x12,0xC6,0x80,0xFF,0x0F,0x00,0x00,0x90,0xCC,0x90,0xC3 };
#endif
    DWORD  oldProt;

    buffer = (BYTE*)VirtualAlloc(NULL, 2 * PAGE_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    /*_asm {
        mov Start, offset start
        mov PageBreak, offset pb
        mov End, offset end1
        jmp end1
    }

    _asm {
    start:
        mov eax, buffer
        mov byte ptr[eax + PAGE_SIZE - 1], 0x90
        int3
    pb:
        nop
        ret
    end1:
    }*/

    //dest = (UINT32)buffer + PAGE_SIZE - (PageBreak - Start);
    //memcpy((void *)dest, (void *)Start, End - Start);
    memcpy(buffer + PAGE_SIZE - (sizeof(code) - 2), code, sizeof(code));
#ifndef _M_X64
    *(DWORD*)(buffer + PAGE_SIZE - (sizeof(code) - 2) + 1) = (DWORD)buffer;
    *(DWORD*)(buffer + PAGE_SIZE - (sizeof(code) - 2) + 7) = (DWORD)(buffer + PAGE_SIZE - 1);
#else
    *(DWORD64*)(buffer + PAGE_SIZE - (sizeof(code) - 2) + 2) = (DWORD64)buffer;
#endif

    VirtualProtect(buffer + PAGE_SIZE, 1, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &oldProt);
    fptr myf = (fptr)(buffer + PAGE_SIZE - (sizeof(code) - 2));
    __try {
        //_asm {
        //  call dword ptr[dest]
        //}
        myf();
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        if (GetExceptionCode() == EXCEPTION_GUARD_PAGE) {
            VirtualFree(buffer, 0, MEM_RELEASE);
            return FALSE;
        }
    }

    VirtualFree(buffer, 0, MEM_RELEASE);
    return TRUE;
}

BOOL TEST_ReadFromCC() {
#ifndef _M_X64
    BYTE code[12] = { 0xD9,0xEE,0x9B,0xD9,0x74,0x24,0xE4,0x8B,0x44,0x24,0xF0,0xC3 };
#else
    BYTE code[15] = { 0xD9,0xEE,0x9B,0xD9,0x74,0x24,0xE4,0x48,0x31,0xC0,0x8B,0x44,0x24,0xF0,0xC3 };
#endif
    printf("[*]Starting read from CC test.. (Hint: disable FPU protection)\n");

    DWORD a;
    /*__asm {
        fldz
        fstenv[esp - 0x1c]
        mov eax, [esp - 0x10]
        mov a, eax
    }*/

    BYTE* buffer = (BYTE*)VirtualAlloc(0, PAGE_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(buffer, code, sizeof(code));
    fptr2 myf = (fptr2)buffer;
    a = myf();

    __try {
        unsigned char m = *(unsigned char*)a;
    }
    __except (1) {//filter(GetExceptionCode(), GetExceptionInformation())) {
        VirtualFree(buffer, 0, MEM_RELEASE);
        return TRUE;
    }

    VirtualFree(buffer, 0, MEM_RELEASE);
    return FALSE;

}

BOOL TEST_FPU() {
#ifndef _M_X64
    BYTE code[22] = { 0xD9,0xEE,0x9B,0xD9,0x74,0x24,0xE4,0x8B,0x44,0x24,0x04,0xC7,0x00,0x78,0x56,0x34,0x12,0x8B,0x44,0x24,0xF0,0xC3 };
#else
    BYTE code[36] = { 0xD9, 0xEE, 0x9B, 0xD9, 0x74, 0x24, 0xE4, 0x48, 0x31, 0xC0, 0x8B, 0x44, 0x24, 0xF0, 0x48, 0xBB, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12, 0x48, 0xC1, 0xE3, 0x20, 0x48, 0xC1, 0xEB, 0x20, 0x48, 0x89, 0x19, 0xC3 };                 
#endif
    printf("[*]Starting FPU test..\n");

    DWORD a, b;
    /*__asm {
    LOC:
        fldz
        fstenv[esp - 0x1c]
        mov eax, [esp - 0x10]
        mov a, eax;
        mov ecx, LOC
        mov b, ecx
    }*/

    BYTE* buffer = (BYTE*)VirtualAlloc(0, PAGE_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(buffer, code, sizeof(code));
#ifndef _M_X64
    *(DWORD*)(buffer + 13) = (DWORD)buffer;
#else
    * (DWORD64*)(buffer + 16) = (DWORD64)buffer;
#endif
    fptr3 myf = (fptr3)buffer;
    a = myf(b);
    VirtualFree(buffer, 0, MEM_RELEASE);

    return (a != b) ? TRUE : FALSE;

}

int main(int argc, char * argv[]) 
{
    /*if (TEST_PageGuard())
        printf(FAIL);
    else
        printf(SUCCESS);*/

    if (TEST_PageGuardAcross())
        printf(FAIL);
    else
        printf(SUCCESS);


    /*if (TEST_ReadFromCC())
        printf(FAIL);
    else
        printf(SUCCESS);

    if (TEST_FPU())
        printf(FAIL);
    else
        printf(SUCCESS);*/

    return ERROR_SUCCESS;
}


{% endraw %}