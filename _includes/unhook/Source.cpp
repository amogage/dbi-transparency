{% raw %}
/*
ARCH: Windows x86
This example unhooks any API hook found in OS libraries and then triggers and exception, forcing the execution of kiUserExceptionDispatcher
While native execution passes the test because no hook exists, any DBI hooking at least 1 API has a high chance to crash
Test should be executed in an OS environment where no EDR/AV sollution exist to inject other supervising modules system-wide
*/

#include <Windows.h>
#include <stdio.h>

//print the hook
void ListHook(BYTE* ex_f)
{
	if (ex_f)
	{
		printf("%p: %02X %02X %02X %02X %02X\n", ex_f, ex_f[0], ex_f[1], ex_f[2], ex_f[3], ex_f[4]);
		if (ex_f[0] == 0xE9)
		{
			DWORD addr = (DWORD)ex_f + 5 + *(DWORD*)(ex_f + 1);
			MEMORY_BASIC_INFORMATION pmem = { 0 };
			VirtualQuery((BYTE*)addr, &pmem, sizeof(pmem));
			printf("%p, %X, %X\n", pmem.BaseAddress, pmem.RegionSize, pmem.Protect);
		}
	}
}

//convert RVA to FA
DWORD RVATOFA(DWORD RVA, BYTE* mod_addr)
{
	IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)mod_addr;
	IMAGE_NT_HEADERS* imnth = (IMAGE_NT_HEADERS*)(mod_addr + imdos->e_lfanew);
	IMAGE_SECTION_HEADER* imsec = (IMAGE_SECTION_HEADER*)((BYTE*)imnth + sizeof(IMAGE_NT_HEADERS));

	for (int i = 0; i < imnth->FileHeader.NumberOfSections; i++)
	{
		if (RVA >= imsec[i].VirtualAddress && RVA < imsec[i].VirtualAddress + imsec[i].Misc.VirtualSize)
			return imsec[i].PointerToRawData + RVA - imsec[i].VirtualAddress;
	}
	return 0;
}

//get address of an API by name
BYTE* GetProcAddressByName(BYTE* mod_base, const char* name)
{
	BYTE* mod_addr = 0;
	DWORD i;
	char* fname = 0;

	mod_addr = mod_base;

	IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)mod_addr;
	IMAGE_NT_HEADERS* imnth = (IMAGE_NT_HEADERS*)(mod_addr + imdos->e_lfanew);
	DWORD rva_exports = imnth->OptionalHeader.DataDirectory[0].VirtualAddress;

	IMAGE_EXPORT_DIRECTORY* imexp = (IMAGE_EXPORT_DIRECTORY*)(RVATOFA(imnth->OptionalHeader.DataDirectory[0].VirtualAddress, mod_addr) + mod_addr);
	DWORD AddressOfNames = imexp->AddressOfNames;
	size_t AddressOfNamesVA = (size_t)(RVATOFA(AddressOfNames, mod_addr) + mod_addr);
	DWORD real_index;
	BYTE* current_addr;

	for (i = 0; i < imexp->NumberOfNames; i++)
	{
		fname = (char*)(RVATOFA(((DWORD*)AddressOfNamesVA)[i], mod_addr) + mod_addr);
		if (!strcmp(fname, name))
		{
			real_index = (size_t)((WORD*)(RVATOFA(imexp->AddressOfNameOrdinals, mod_addr) + mod_addr))[i];
			current_addr = (BYTE*)(RVATOFA(((DWORD*)(RVATOFA(imexp->AddressOfFunctions, mod_addr) + mod_addr))[real_index], mod_addr) + mod_addr);
			return current_addr;
		}
	}

	return 0;
}

int main()
{
	DWORD oldp = 0;
	char fpath[MAX_PATH] = { 0 };
	HMODULE hm = GetModuleHandle("ntdll.dll");
	BYTE* ex_f1 = (BYTE*)GetProcAddress(hm, "KiUserExceptionDispatcher");
	BYTE* ex_f2 = (BYTE*)GetProcAddress(hm, "KiUserApcDispatcher");
	BYTE* ex_f3 = (BYTE*)GetProcAddress(hm, "KiUserCallbackDispatcher");
	BYTE* ex_f4 = (BYTE*)GetProcAddress(hm, "LdrInitializeThunk");

	ListHook(ex_f1);
	ListHook(ex_f2);
	ListHook(ex_f3);
	ListHook(ex_f4);

	HMODULE ntdll = GetModuleHandle("ntdll.dll");
	GetModuleFileName(ntdll, fpath, MAX_PATH);
	HANDLE f = CreateFile(fpath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (f == INVALID_HANDLE_VALUE)
	{
		printf("cannot access [%s]\n", fpath);
		return 0;
	}
	DWORD size = GetFileSize(f, 0);
	DWORD read = 0;
	BYTE* buffer = (BYTE*)malloc(size);
	ReadFile(f, buffer, size, &read, 0);
	CloseHandle(f);

	BYTE* ad1 = GetProcAddressByName(buffer, "KiUserExceptionDispatcher");
	BYTE* ad2 = GetProcAddressByName(buffer, "KiUserApcDispatcher");
	BYTE* ad3 = GetProcAddressByName(buffer, "KiUserCallbackDispatcher");
	BYTE* ad4 = GetProcAddressByName(buffer, "LdrInitializeThunk");

	printf("%X %X %X %X\n", ad1, ad2, ad3, ad4);
	
	if (ex_f1[0] == 0xE9)
	{
		printf("apply for [KiUserExceptionDispatcher]\n");
		VirtualProtect(ex_f1, 5, PAGE_READWRITE, &oldp);
		memcpy(ex_f1, ad1, 5);
		VirtualProtect(ex_f1, 5, oldp, &oldp);
	}

	if (ex_f2[0] == 0xE9)
	{
		printf("apply for [KiUserApcDispatcher]\n");
		VirtualProtect(ex_f2, 5, PAGE_READWRITE, &oldp);
		memcpy(ex_f2, ad2, 5);
		VirtualProtect(ex_f2, 5, oldp, &oldp);
	}

	if (ex_f3[0] == 0xE9)
	{
		printf("apply for [KiUserCallbackDispatcher]\n");
		VirtualProtect(ex_f3, 5, PAGE_READWRITE, &oldp);
		memcpy(ex_f3, ad3, 5);
		VirtualProtect(ex_f3, 5, oldp, &oldp);
	}

	if (ex_f4[0] == 0xE9)
	{
		printf("apply for [LdrInitializeThunk]\n");
		VirtualProtect(ex_f4, 5, PAGE_READWRITE, &oldp);
		memcpy(ex_f4, ad4, 5);
		VirtualProtect(ex_f4, 5, oldp, &oldp);
	}	

	ListHook(ex_f1);
	ListHook(ex_f2);
	ListHook(ex_f3);
	ListHook(ex_f4);

	__try {
		//intentionally trigger an exception to force the execution of kiUserExceptionDispatcher which is not hooked anymore
		DebugBreak();
	}
	__except (1) {
		//if DBI hook on kiUserExceptionDispatcher was cleared, this message will not be shown, and the instrumentation crashes
		printf("PASSED");
	}

	return 0;
}
{% endraw %}