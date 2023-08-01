{% raw %}
/*
ARCH: Windows x86
This example cheks if the DBI use multiple address spaces for the same thread.
If thread context is not properly virtualized, at some point we may find the DBI-cache instead of the actual thread address space
Test fails if more than 1 address space relates to the same thread, on multiple thread suspend/resume operations
*/

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <map>
#include <TlHelp32.h>
using namespace std;

#pragma warning (disable:4996)

typedef LONG(NTAPI* NtSuspendProcess)(HANDLE);
typedef LONG(NTAPI* NtResumeProcess)(HANDLE);

DWORD WINAPI MegaSqrt(void* param)
{
    LARGE_INTEGER start, stop, freq;
    double timp;
    DWORD curent;
    double r;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (size_t i = 0; i < 10000000; i++)
    {
        if ((i % 100000) == 0)
            printf(".");
        curent = (DWORD)__rdtsc();
        //r = squareRoot(curent);
        double z = curent;
        double rst = 0.0;
        int max = 8;
        int ii;
        double j = 1.0;
        for (ii = max; ii > 0; ii--)
        {
            if (z - ((2 * rst) + (j * pow(ii, 10))) * (j * pow(ii, 10)) >= 0)
            {
                while (z - ((2 * rst) + (j * pow(ii, 10))) * (j * pow(ii, 10)) >= 0)
                {
                    j++;
                    if (j >= 10) break;

                }
                j--;
                z -= ((2 * rst) + (j * pow(ii, 10))) * (j * pow(ii, 10)); //find value of z

                rst += j * pow(ii, 10);

                j = 1.0;
            }
        }

        for (ii = 0; ii >= 0 - max; ii--)
        {
            if (z - ((2 * rst) + (j * pow(ii, 10))) * (j * pow(ii, 10)) >= 0)
            {
                while (z - ((2 * rst) + (j * pow(ii, 10))) * (j * pow(ii, 10)) >= 0)
                {
                    j++;
                    if (j >= 10) break;
                }
                j--;
                z -= ((2 * rst) + (j * pow(ii, 10))) * (j * pow(ii, 10));

                rst += j * pow(ii, 10);
                j = 1.0;
            }
        }

        r = rst;
    }
    QueryPerformanceCounter(&stop);
    timp = (stop.QuadPart - start.QuadPart) / (double)freq.QuadPart;
    printf("timp = %.3lf\n", timp);
    return 0;
}

int main(int argc, char* argv[])
{
    CONTEXT ctx;
    size_t start = (size_t)MegaSqrt;
    size_t finis = (size_t)MegaSqrt + 0x4DE;
    map<size_t, pair<DWORD, DWORD>> mems;

    printf("limits: %08X - %08X\n", MegaSqrt, (DWORD)MegaSqrt + 0x4DE);

    HANDLE thrd = CreateThread(0, 0, MegaSqrt, 0, 0, 0);
    for (int i = 0; i < 100; i++)
    {
        Sleep(100);
        SuspendThread(thrd);
        ctx.ContextFlags = CONTEXT_ALL;
        GetThreadContext(thrd, &ctx);
#ifndef _M_X64
        MEMORY_BASIC_INFORMATION bb = { 0 };
        VirtualQuery((void*)ctx.Eip, &bb, sizeof(bb));
#else
        MEMORY_BASIC_INFORMATION bb = { 0 };
        VirtualQuery((void*)ctx.Rip, &bb, sizeof(bb));
#endif
        if (mems.find((size_t)bb.AllocationBase) == mems.end())
        {
            if (!((size_t)bb.AllocationBase & 0xF0000000))
                mems[(size_t)bb.AllocationBase] = { bb.RegionSize + (DWORD)bb.BaseAddress - (DWORD)bb.AllocationBase , bb.Protect};
        }

        ResumeThread(thrd);
    }
    TerminateThread(thrd, 0);

    printf("\n\n");
    for (auto it : mems)
        printf("ELEMENT: %p:%08X:%08X\n", it.first, it.second.first, it.second.second);

    if (mems.size() == 1)
        printf("PASSED");
    else
        printf("FAILED");

    return 0;
}
{% endraw %}