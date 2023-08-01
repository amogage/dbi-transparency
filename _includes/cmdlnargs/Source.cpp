{% raw %}
/*
ARCH: Windows x86/x64
Tis example compares te format of cmdline between native execution and DBI instrmentation
Usually, thare are some changes depending on how DBIs pass the cmdline to the child processes
*/

#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>

int main()
{
	CHAR* cmdln = GetCommandLine();
	printf("[%s]\n", cmdln);
	PathRemoveFileSpec(cmdln);
	if (strlen(cmdln))
		printf("PASSED");
	else
		printf("FAILED");
	return 0;
}


{% endraw %}