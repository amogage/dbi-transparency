{% raw %}
/*
ARCH: Windows x86
This example exploits a behaviour difference between the CPU-cache and the DBI-cache, when self modfying code is used
The full story of this test can be found here: 
https://github.com/fplu/Detector
https://link.springer.com/article/10.1007/s11416-020-00371-x
*/

#include <Windows.h>
#include <stdio.h>

BOOLEAN Sync = FALSE;

extern int ModifiedThread();
extern void ThreadWhichModify();

int main(int argc, char** argv) {

	HANDLE hThread;
	DWORD oldProtect;
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	//Change the page right to allow both execution and writing.
	if (VirtualProtect(ModifiedThread, si.dwPageSize - 0xF, PAGE_EXECUTE_READWRITE, &oldProtect) == 0) {
		return EXIT_FAILURE;
	}

	//Create a new thread to which will modify the code to execute.
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadWhichModify, NULL, 0, NULL);
	if (hThread == NULL) {
		return EXIT_FAILURE;
	}


	//Execute the function that is modified and display its result.
	int flg = ModifiedThread();
	if (!flg)
		printf("PASSED");
	else
		printf("FAILED");

	//Restore the right of the page.
	VirtualProtect(ModifiedThread, si.dwPageSize - 0xF, oldProtect, &oldProtect);
	WaitForSingleObject(ModifiedThread, INFINITE);

	return EXIT_SUCCESS;
}
{% endraw %}