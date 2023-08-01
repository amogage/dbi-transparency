{% raw %}
/*
ARCH: Windows x86
This example allocates a lot of memory and snapshots the largest available memory taken from a native execution and compares it with those oobtained when instrmented
Usually when DBI is used, these limits are easily exceeded
*/

#include <windows.h>
#include <stdio.h>

#define mem_limit_simple 0x80000000
#define mem_limit_large  0xFF000000

int main()
{
	size_t i = 1;
	BYTE* large_mem = 0;
    DWORD size = 0x100000;
    DWORD s = 0;
    size_t mem_limit;
    srand(GetTickCount());
	do {
		large_mem = (BYTE*)malloc(size * i);
        if (large_mem)
            memset(large_mem, rand() & 0xFF, size);
		i++;
        s += size * i;
	} while (large_mem != 0);
	
    BYTE* cmod = (BYTE*)GetModuleHandle(0);
    IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)cmod;
    IMAGE_NT_HEADERS* imnt = (IMAGE_NT_HEADERS*)(cmod + imdos->e_lfanew);
    if (imnt->FileHeader.Characteristics & IMAGE_FILE_LARGE_ADDRESS_AWARE)
        mem_limit = mem_limit_large;
    else
        mem_limit = mem_limit_simple;

    if (s >= mem_limit)
        printf("PASSED");
    else
        printf("FAILED");

	return 0;
}

{% endraw %}