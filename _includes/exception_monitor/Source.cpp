{% raw %}
/*
ARCH: Windows x86/x64
This example includes a mix of all possible low-level exceptions to test the DBI abbility to deal wit such scenarios
- SEH
- VEH
- Unhandled
*/

#include <windows.h>
#include <stdio.h>

#define CALL_FIRST 1  
#define CALL_LAST 0

LPTOP_LEVEL_EXCEPTION_FILTER oldf;

void Unhandled_Redirect_Exception();
BYTE filters[5] = { 0 };
BYTE exception_behaviors[5] = { 0 };

int seh_dummy_filter(unsigned int code, struct _EXCEPTION_POINTERS* ep)
{
#ifndef _M_X64
    printf("Simple dummy SEH Filter: Do Nothing: %p, %p\n", ep->ExceptionRecord->ExceptionAddress, ep->ContextRecord->Eip);
    if ((size_t)ep->ExceptionRecord->ExceptionAddress == (size_t)ep->ContextRecord->Eip)
#else
    printf("Simple dummy SEH Filter: Do Nothing: %p, %p\n", ep->ExceptionRecord->ExceptionAddress, ep->ContextRecord->Rip);
    if ((size_t)ep->ExceptionRecord->ExceptionAddress == (size_t)ep->ContextRecord->Rip)
#endif
        filters[0] = 1;
    return EXCEPTION_EXECUTE_HANDLER;
}

LONG WINAPI Dummy_VectoredHandler(struct _EXCEPTION_POINTERS* ep)
{
#ifndef _M_X64
    printf("Simple dummy VEH Filter: Do Nothing: %p, %p\n", ep->ExceptionRecord->ExceptionAddress, ep->ContextRecord->Eip);
    if ((size_t)ep->ExceptionRecord->ExceptionAddress == (size_t)ep->ContextRecord->Eip)
#else
    printf("Simple dummy VEH Filter: Do Nothing: %p, %p\n", ep->ExceptionRecord->ExceptionAddress, ep->ContextRecord->Rip);
    if ((size_t)ep->ExceptionRecord->ExceptionAddress == (size_t)ep->ContextRecord->Rip)
#endif
        filters[1] = 1;
    return EXCEPTION_CONTINUE_EXECUTION;
}

LONG WINAPI veh_redirect_filter(struct _EXCEPTION_POINTERS* ep)
{
#ifndef _M_X64
    printf("Redirect VEH Filter: Do Nothing: %p, %p --> %p\n", ep->ExceptionRecord->ExceptionAddress, ep->ContextRecord->Eip, ep->ContextRecord->Eip + 0x87);
    if ((size_t)ep->ExceptionRecord->ExceptionAddress == (size_t)ep->ContextRecord->Eip)
#else
    printf("Redirect VEH Filter: Do Nothing: %p, %p --> %p\n", ep->ExceptionRecord->ExceptionAddress, ep->ContextRecord->Rip, ep->ContextRecord->Rip + 0x7F);
    if ((size_t)ep->ExceptionRecord->ExceptionAddress == (size_t)ep->ContextRecord->Rip)
#endif
        filters[2] = 1;
#ifndef _M_X64
    ep->ContextRecord->Eip += 0x87;
#else
    ep->ContextRecord->Rip += 0x7F;
#endif
    return EXCEPTION_CONTINUE_EXECUTION;
}

LONG WINAPI Dummy_Top_ExceptionFilter(struct _EXCEPTION_POINTERS* ep)
{        
#ifndef _M_X64
    printf("Simple dummy UNHANDLED Filter: Do Nothing: %p, %p --> %p\n", ep->ExceptionRecord->ExceptionAddress, ep->ContextRecord->Eip, ep->ContextRecord->Eip + 1);
    if ((size_t)ep->ExceptionRecord->ExceptionAddress == (size_t)ep->ContextRecord->Eip)
#else
    printf("Simple dummy UNHANDLED Filter: Do Nothing: %p, %p --> %p\n", ep->ExceptionRecord->ExceptionAddress, ep->ContextRecord->Rip, ep->ContextRecord->Rip + 1);
    if ((size_t)ep->ExceptionRecord->ExceptionAddress == (size_t)ep->ContextRecord->Rip)
#endif
        filters[3] = 1;
#ifndef _M_X64
    ep->ContextRecord->Eip++;
#else
    ep->ContextRecord->Rip++;
#endif
    return EXCEPTION_CONTINUE_EXECUTION;
}

LONG WINAPI Redirect_Top_ExceptionFilter(struct _EXCEPTION_POINTERS* ep)
{
#ifndef _M_X64
    printf("Simple redirect UNHANDLED Filter: Do Nothing: %p, %p --> %p\n", ep->ExceptionRecord->ExceptionAddress, ep->ContextRecord->Eip, (DWORD)Unhandled_Redirect_Exception + 0xA7);
    if ((size_t)ep->ExceptionRecord->ExceptionAddress == (size_t)ep->ContextRecord->Eip)
#else
    printf("Simple redirect UNHANDLED Filter: Do Nothing: %p, %p --> %p\n", ep->ExceptionRecord->ExceptionAddress, ep->ContextRecord->Rip, (DWORD)Unhandled_Redirect_Exception + 0xA4);
    if ((size_t)ep->ExceptionRecord->ExceptionAddress == (size_t)ep->ContextRecord->Rip)
#endif
        filters[4] = 1;
#ifndef _M_X64
    ep->ContextRecord->Eip = (size_t)Unhandled_Redirect_Exception + 0xA7;
#else
    ep->ContextRecord->Rip = (size_t)Unhandled_Redirect_Exception + 0xA4;
#endif
    return EXCEPTION_CONTINUE_EXECUTION;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void SEH_Dummy_Exception()
{
    __try {
        int invalid_ptr = 100;
        BYTE x = *(BYTE*)invalid_ptr;
    }
    __except (seh_dummy_filter(GetExceptionCode(), GetExceptionInformation())) {
        printf("Exception: Correct dummy SEH Handled\n");
        exception_behaviors[0] = 1;
    }
}

void VEH_Dummy_Exception()
{
    PVOID h;
    h = AddVectoredExceptionHandler(CALL_LAST, Dummy_VectoredHandler);
    RaiseException(1, 0, 0, NULL);
    printf("Exception: Correct dummy VEH Handled\n");
    exception_behaviors[1] = 1;
    RemoveVectoredExceptionHandler(h);
}

void Unhandled_Dummy_Exception()
{
    oldf = SetUnhandledExceptionFilter(Dummy_Top_ExceptionFilter);
    DebugBreak();
    printf("Exception: Correct dummy UNHANDLED Handled\n");
    exception_behaviors[3] = 1;
    SetUnhandledExceptionFilter(oldf);
}

void VEH_Redirect_Exception()
{
    PVOID h = AddVectoredExceptionHandler(CALL_LAST, veh_redirect_filter);
    int invalid_ptr = 100;    
    BYTE x = *(BYTE*)invalid_ptr;
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: Correct VEH redirect Handled\n");
    exception_behaviors[2] = 1;
    RemoveVectoredExceptionHandler(h);
}

void Unhandled_Redirect_Exception()
{
    oldf = SetUnhandledExceptionFilter(Redirect_Top_ExceptionFilter);
    int invalid_ptr = 100;
    BYTE x = *(BYTE*)invalid_ptr;
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: SEH redirect : This should not be executed\n");
    printf("Exception: Correct redirect UNHANDLED Handled\n");
    exception_behaviors[4] = 1;
    SetUnhandledExceptionFilter(oldf);
}

int main()
{
    SEH_Dummy_Exception();
    VEH_Dummy_Exception();
    Unhandled_Dummy_Exception();

    VEH_Redirect_Exception();
    Unhandled_Redirect_Exception(); 

    bool passed = true;
    for (int i = 0; i < 5; i++)
    {
        if (!filters[i])
            passed = false;
        if (!exception_behaviors[i])
            passed = false;
    }

    if (passed)
        printf("PASSED");
    else
        printf("FAILED");

    return 0;
}


{% endraw %}