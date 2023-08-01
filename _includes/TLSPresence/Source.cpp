{% raw %}
/*
ARCH: Windows x86
This example fingerprints TLS memory regions specific to PIN and DynamoRIO, based on constant strings
Both PIN and DynamoRIO fails
Credits to:
https://www.blackhat.com/docs/asia-16/materials/asia-16-Sun-Break-Out-Of-The-Truman-Show-Active-Detection-And-Escape-Of-Dynamic-Binary-Instrumentation.pdf
https://www.youtube.com/watch?v=VGmvx2B5qdo
*/

#include <windows.h>
#include <stdio.h>

#pragma warning (disable:4996)

int main()
{
	size_t buffer[64] = { 0 };
	size_t compare[64] = { 0 };
	char detect[4][300] = {
		"DynamoRIO developers",
		"dynamorio_annotate_running_on_dynamorio",
		"dynamorio.dll",
		"======================================================================================\nORIGINAL ADDRESS  CODE CACHE ADDRESS           COUNT     APP    TOOL     PIN    LINK\n======================================================================================\n"
	};

	for (int i = 0; i < TLS_MINIMUM_AVAILABLE; i++)
	{
		buffer[i] = (size_t)TlsGetValue(i);
		if (buffer[i])
			printf("%d. %p\n", i, buffer[i]);
	}

	if (!memcmp(buffer, compare, 64 * sizeof(DWORD)))
		printf("PASSED");
	else
	{
		for (int i = 0; i < TLS_MINIMUM_AVAILABLE; i++)
		{
			if (buffer[i])
			{
				MEMORY_BASIC_INFORMATION pmem = { 0 };
				VirtualQuery((void*)buffer[i], &pmem, sizeof(pmem));
				if (pmem.BaseAddress && pmem.RegionSize && pmem.State == MEM_COMMIT)
				{
					char fname[100];
					DWORD write;
					sprintf(fname, "%p_%08X.bin", pmem.BaseAddress, pmem.RegionSize);
					HANDLE f = CreateFile(fname, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
					WriteFile(f, pmem.BaseAddress, pmem.RegionSize, &write, 0);
					CloseHandle(f);

					for (int j = 0; j < pmem.RegionSize - 1000; j++)
					{
						if (*(DWORD*)((BYTE*)pmem.BaseAddress + j + 1) == 'many')
						{
							for (int k = 0; k < 3; k++)
							{
								if (!memcmp((BYTE*)pmem.BaseAddress + j, detect[k], strlen(detect[k])))
								{
									printf("%p: [%d][%s]\n", pmem.BaseAddress, j, detect[k]);
									break;
								}
							}
						}
						if (!strncmp((char*)pmem.BaseAddress + j, detect[3], 86))
						{
							printf("%p: [%d][%s]\n", pmem.BaseAddress, j, detect[3]);
							break;
						}
					}
				}
			}
		}
		printf("FAILED");
	}
	
	return 0;
}

{% endraw %}