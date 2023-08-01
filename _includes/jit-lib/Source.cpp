{% raw %}
/*
ARCH: Windows x86
This example measures difference between consecutive instrumentation of library load, where usually DBIs add a significant overhead
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
*/

#include <windows.h>
#include <stdio.h>
#include <intrin.h>

#pragma warning (disable:4996)

#define NUM_LIBS 5
#define NUM_LOADS 5
#define MAXIMUM_LOAD_DIFF 300000000

unsigned long long start[NUM_LOADS], end[NUM_LOADS];
const static char* windows_common_libs[NUM_LIBS] = { "WindowsCodecsRaw.dll", "vm3dgl.dll", "edgehtml.dll", "mshtml.dll", "DXCaptureReplay.dll" };

int main()
{
    for (int i = 0; i < NUM_LOADS; i++) 
    {
        start[i] = __rdtsc();

        for (int j = 0; j < NUM_LIBS; j++)
        {
            HMODULE hm = LoadLibrary(windows_common_libs[j]);
            FreeLibrary(hm);
        }

        end[i] = __rdtsc();
    }

    for (int i = 0; i < NUM_LOADS; i++) 
    {
        if (end[i] < start[i]) 
        {
            printf("FAILED");
            return 0;
        }
    }

    long long loads[NUM_LOADS] = { 0 };
    int points = 0;
    for (int i = 0; i < NUM_LOADS; i++)
    {
        loads[i] = end[i] - start[i];
        printf("Difference:\t%.3ld\n", loads[i]);
        if (loads[i] > MAXIMUM_LOAD_DIFF)
            points++;
    }

    printf("points: %d\n", points);

    if (!points)
        printf("PASSED");
    else
        printf("FAILED");

    return 0;
}
{% endraw %}