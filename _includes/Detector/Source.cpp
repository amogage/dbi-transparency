{% raw %}
/*
This example exploits a CPU cache behavior commpared to DBIs, executing a behavior on physical CPU and a different one in DBIs and analysis environments
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
		printf("PASSED\n");
	else
		printf("FAILED\n");

	//Restore the right of the page.
	VirtualProtect(ModifiedThread, si.dwPageSize - 0xF, oldProtect, &oldProtect);
	WaitForSingleObject(ModifiedThread, INFINITE);

	return EXIT_SUCCESS;
}
{% endraw %}