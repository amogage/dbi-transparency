{% raw %}
/*
ARCH: Windows x86
This example measures difference between consecutive branch instrumentation, where usually DBIs add a significant overhead
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
*/

#include <windows.h>
#include <stdio.h>
#include <intrin.h>

#pragma warning (disable:4996)

int main(void)
{
    unsigned long long times[10];
    times[0] = __rdtsc();

    for (int i = 1; i < 10; i++) {
        times[i] = __rdtsc();
    }

    /*
        The first trace which PIN compiles includes both 0 and 1 times.
        Since the for loop jumps in the middle of a compiled trace, although
        the code is already in code cache, the traces are not equal, so it
        has to be jitted again (1-2). Lastly, there exist a trace with the
        same start address in the code cache but the static context has to be
        regenerated again before execution (2-3). So, only after the 4. rdtsc
        execution, PIN uses exclusively the instructions residing in code cache.
    */
    long long jit_time = max(max(times[2] - times[1], times[3] - times[2]), times[4] - times[3]);

    long long current, diff[8] = { 0 };
    long long maxim = 0;
    for (int i = 0; i < 10; i++) 
        printf("%d\n", times[i] & 0xFFFF);

    for (int i = 2; i < 10; i++)
    {
        diff[i - 2] = times[i] - times[i - 1];
        maxim = max(maxim, diff[i - 2]);
    }

    printf("\nJIT time:\t\t%lld\n", maxim);
    for (int i = 0; i < 8; i++)
        printf("Reused cache time:\t%lld\n", diff[i]);

    if (maxim > 100)
        printf("\nFAILED");
    else
        printf("\nPASSED");

    return 0;
}
{% endraw %}