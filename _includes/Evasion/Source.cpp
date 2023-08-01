{% raw %}
/*
ARCH: Windows x86/x64
This example charts te process address space to {start_addr -> {size, access}} using only exceptions, no API at all.
The excessive usage of exceptions has 2 negative side-effects on DBIs
1. a signifficant increase of execution overhead
2. usage of nested exceptions has a high cance of generating execution crashes
Te test is considered PASSED if application does not crash during instrumentation
*/

#include <windows.h>
#include <string>
#include <map>
#include <set>
#include <vector>
using namespace std;

#pragma warning (disable:4996)
typedef void(*dummy_f)();

#define INSTR_RET 0xC3
#define MZPE_MAGIC 0x5A4D
#define DOS_LFANEW_OFFSET 0x3C

struct AREA 
{
	BYTE* addr;
	DWORD size;
	int flags;
};

AREA chunks[10000];
int index;

//if we got ere, it means tat addr exists
int GetAccessRights(BYTE* addr)
{
	int access = 0;
	//backup existing value
	BYTE read = addr[0x800];
	__try {
		//try to write
		addr[0x800] = 0x11;
		__try {			
			//if write works, try execute
			addr[0x800] = INSTR_RET;
			((dummy_f)addr)();
			access = PAGE_EXECUTE_READWRITE;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			//execute did not worked, let it be ReadWrite
			access = PAGE_READWRITE;
		}
		//restore backed-up value
		addr[0x800] = read;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		//Write didn't worked, so Read
		int i = 0;
		for (i = 0; i < 0x1000; i++)
		{
			if (addr[i] == INSTR_RET)
				break;
		}
		if (i < 0x1000)
		{
			//try execute
			__try {
				((dummy_f)(addr + i))();
				access = PAGE_EXECUTE_READ;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				//ReadOnly then
				access = PAGE_READONLY;
			}
		}
		else
			access = PAGE_READONLY;
	}	
	return access;
}

//Scan all memory regions, using exceptions ana a specified granularity for chunk size
void ScanMemRangeWithGranularity(size_t m_start, size_t m_finish, size_t increment, int idx)
{
	BYTE* start = (BYTE*)m_start;
	index = idx;
	int start_flag, second_flag;

	while (true)
	{
		//try accessing chunk at m_start
		__try {
			BYTE crt = start[0];
			start_flag = GetAccessRights(start);
			BYTE* works = start;
			DWORD size = 0;
			while (true)
			{
				__try {
					//find the limit of this chunk
					start += 0x1000;
					size += 0x1000;
					crt = start[0];

					if ((size_t)start >= (m_finish & -4096))
						break;
					second_flag = GetAccessRights(start);
					if (second_flag != start_flag)
					{
						chunks[index++] = { works, size, start_flag };
						start_flag = second_flag;
						works = start;
						size = 0;
						if (index == idx + 1000)
							break;
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER) {
					//chunk finished
					chunks[index++] = { works, size, start_flag };
					break;
				}
			}
			if ((size_t)start >= (m_finish & -4096))
				break;
			if (index == idx + 1000)
				break;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			start += increment;
			if ((size_t)start >= (m_finish & -4096))
				break;
		}
	}
}

//map consecutive memory regions, to contigous memory cunks, inclusing modules sections
map<BYTE*, pair<DWORD, string>> MapAddressSpace()
{
	map<BYTE*, pair<DWORD, string>> addr_space;
	int i = 0;
	int j = 0;
	while (i<index)
	{
		if (chunks[i].flags == PAGE_READONLY && *(WORD*)chunks[i].addr == MZPE_MAGIC && *(DWORD*)(chunks[i].addr + DOS_LFANEW_OFFSET) < 0x1000)
		{
			IMAGE_DOS_HEADER* imdos = (IMAGE_DOS_HEADER*)chunks[i].addr;
			IMAGE_FILE_HEADER* imfh = (IMAGE_FILE_HEADER*)(chunks[i].addr + imdos->e_lfanew + 4);
			IMAGE_SECTION_HEADER* imsec = 0;
			if (imfh->Machine == 0x14C)
			{
				IMAGE_NT_HEADERS32* imnt = (IMAGE_NT_HEADERS32*)(chunks[i].addr + imdos->e_lfanew);
				imsec = (IMAGE_SECTION_HEADER*)((BYTE*)imnt + sizeof(IMAGE_NT_HEADERS32));
				addr_space[chunks[i].addr] = { chunks[i].size, "R" };
				for (int j = 0; j < imnt->FileHeader.NumberOfSections; j++)
				{
					if ((imsec[j].Characteristics & 0xF0000000) == (IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ))
						addr_space[chunks[i].addr + imsec[j].VirtualAddress] = { (imsec[j].Misc.VirtualSize / 0x1000 + 1) * 0x1000 , "RE" };
					else
						if ((imsec[j].Characteristics & 0xF0000000) == IMAGE_SCN_MEM_READ)
							addr_space[chunks[i].addr + imsec[j].VirtualAddress] = { (imsec[j].Misc.VirtualSize / 0x1000 + 1) * 0x1000 , "R" };
						else
							if ((imsec[j].Characteristics & 0xF0000000) == (IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE))
								addr_space[chunks[i].addr + imsec[j].VirtualAddress] = { (imsec[j].Misc.VirtualSize / 0x1000 + 1) * 0x1000 , "RW" };
							else
								if ((imsec[j].Characteristics & 0xF0000000) == (IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE))
									addr_space[chunks[i].addr + imsec[j].VirtualAddress] = { (imsec[j].Misc.VirtualSize / 0x1000 + 1) * 0x1000 , "RWE" };
				}
			}
			else
				if (imfh->Machine == 0x8664)
				{
					IMAGE_NT_HEADERS64* imnt = (IMAGE_NT_HEADERS64*)(chunks[i].addr + imdos->e_lfanew);
					imsec = (IMAGE_SECTION_HEADER*)((BYTE*)imnt + sizeof(IMAGE_NT_HEADERS64));
					addr_space[chunks[i].addr] = { chunks[i].size, "R" };
					for (j = 0; j < imnt->FileHeader.NumberOfSections; j++)
					{
						if ((imsec[j].Characteristics & 0xF0000000) == (IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ))
							addr_space[chunks[i].addr + imsec[j].VirtualAddress] = { (imsec[j].Misc.VirtualSize / 0x1000 + 1) * 0x1000 , "RE" };
						if ((imsec[j].Characteristics & 0xF0000000) == IMAGE_SCN_MEM_READ)
							addr_space[chunks[i].addr + imsec[j].VirtualAddress] = { (imsec[j].Misc.VirtualSize / 0x1000 + 1) * 0x1000 , "R" };
						if ((imsec[j].Characteristics & 0xF0000000) == (IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE))
							addr_space[chunks[i].addr + imsec[j].VirtualAddress] = { (imsec[j].Misc.VirtualSize / 0x1000 + 1) * 0x1000 , "RW" };
						if ((imsec[j].Characteristics & 0xF0000000) == (IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE))
							addr_space[chunks[i].addr + imsec[j].VirtualAddress] = { (imsec[j].Misc.VirtualSize / 0x1000 + 1) * 0x1000 , "RWE" };
					}
				}
			while (chunks[i].addr < ((--addr_space.end())->first + (--addr_space.end())->second.first))
				i++;
			continue;
		}
		else
		{
			addr_space[chunks[i].addr].first = chunks[i].size;
			if (chunks[i].flags == PAGE_READONLY)
				addr_space[chunks[i].addr].second = "R";
			else
				if (chunks[i].flags == PAGE_EXECUTE_READ)
					addr_space[chunks[i].addr].second = "RE";
				else
					if (chunks[i].flags == PAGE_READWRITE)
						addr_space[chunks[i].addr].second = "RW";
					else
						if (chunks[i].flags == PAGE_EXECUTE_READWRITE)
							addr_space[chunks[i].addr].second = "RWE";
		}
		i++;
	}
	return addr_space;
}

int main()
{
#ifndef _M_X64
	ScanMemRangeWithGranularity(0, -1, 0x1000, 0);
	map<string, map<BYTE*, DWORD>> by_access;

	//get the address space map
	map<BYTE*, pair<DWORD, string>> mapping = MapAddressSpace();

	//map chunks by access rights
	for (auto it : mapping)
		by_access[it.second.second][it.first] = it.second.first;

	//show ow many ranges exist per access rights
	printf("Total ranges: %d\n", (int)mapping.size());
	for (auto it : by_access)
		printf("%s : [%d]\n", it.first.c_str(), it.second.size());
#else
	printf("[00000000 00000000] - [00000000 FFFFFFFF]\n");
	ScanMemRangeWithGranularity(0, 0xFFFFFFFF, 0x1000, 0);
	for (int i = 0; i < index; i++)
		printf("%p - %X - %X\n", chunks[i].addr, chunks[i].size, chunks[i].flags);
	printf("[00000010 00000000] - [000000FF FFFFFFFF]\n");
	ScanMemRangeWithGranularity(0x1000000000, 0xFFFFFFFFFF, 0x10000, 1000);
	for (int i = 1000; i < index; i++)
		printf("%p - %X - %X\n", chunks[i].addr, chunks[i].size, chunks[i].flags);
	printf("[00000100 00000000] - [00000FFF FFFFFFFF]\n");
	ScanMemRangeWithGranularity(0x10000000000, 0xFFFFFFFFFFF, 0x100000, 2000);
	for (int i = 2000; i < index; i++)
		printf("%p - %X - %X\n", chunks[i].addr, chunks[i].size, chunks[i].flags);
	printf("[00001000 00000000] - [0000FFFF FFFFFFFF]\n");
	ScanMemRangeWithGranularity(0x100000000000, 0x7FFFFFFFFFFF, 0x100000, 3000);
	for (int i = 3000; i < index; i++)
		printf("%p - %X - %X\n", chunks[i].addr, chunks[i].size, chunks[i].flags);
#endif

	//if application fails, it triggers an application crash
	printf("PASSED");
	return 0;
}


{% endraw %}