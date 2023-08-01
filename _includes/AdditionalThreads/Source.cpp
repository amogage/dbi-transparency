{% raw %}
/*
ARCH: Windows x86
This example counts the number of threads inside the process.
We try to reveal that a DBI or an analysis system use additional threads for various reasons
*/

#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>

#pragma warning (disable:4996)

//scan existing threads for a given pid
int GetNrThreads(int target_pid)
{
    int threads = 0;
    THREADENTRY32 thrd = { 0 };

    if (!target_pid)
        target_pid = GetCurrentProcessId();

    HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, target_pid);
    thrd.dwSize = sizeof(thrd);
    if (Thread32First(h, &thrd))
    {
        do {
            if (thrd.th32OwnerProcessID == target_pid)
            {
                printf("owner:%d, tid:%d\n", thrd.th32OwnerProcessID, thrd.th32ThreadID);
                threads++;
            }
        } while (Thread32Next(h, &thrd));
    }
    printf("\n");
    return threads;
}

int main(int argc, char* argv[])
{   
    int threads1 = GetNrThreads(0);
    printf("Threads: %d\n", threads1);

    if (threads1 > 4 || threads1 < 3)
        printf("FAILED");
    else
        printf("PASSED");

    return 0;
}

{% endraw %}