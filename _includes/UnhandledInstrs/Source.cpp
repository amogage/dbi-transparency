{% raw %}
/*
ARCH: Windows x86
This example use a mix of exceptions and possible new or CPU instructions from other architectures to see how DBI handles them
While DynamoRIO passes the test, PIN fails
*/

#include <stdio.h>
#include <windows.h>
#include <intrin.h>
#include <immintrin.h>

int main()
{
    unsigned int index = 0;
    __m256i a = { 0 };
    __m256i b = { 0 };
    __m256i c = { 0 };
    int result[5] = { 0 };
    int ea, ed, ec;

    __try {
        _mm256_alignr_epi32(a, b, 100);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        result[0] = _exception_code();
        printf("%X\n", result[0]);
    }

    __try {
        _rdpkru_u32();
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        result[1] = _exception_code();
        printf("%X\n", result[1]);
    }

    __try {
        _xend();
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        result[2] = _exception_code();
        printf("%X\n", result[2]);
    }

    __try {
        _xabort(100);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        result[3] = _exception_code();
        printf("%X\n", result[3]);
    }

    __try {
        _mm256_dpwssds_avx_epi32(a, b, c);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        result[4] = _exception_code();
        printf("%X\n", result[4]);
    }   

    bool passed = true;
    for (int i = 0; i < 5; i++)
    {
        if (result[i] != STATUS_ILLEGAL_INSTRUCTION)
        {
            printf("%d ", result[i]);
            passed = false;
        }
    }

    if (passed)
        printf("PASSED");
    else
        printf("FAILED");

    return 0;
}


{% endraw %}