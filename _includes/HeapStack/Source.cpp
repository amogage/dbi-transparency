{% raw %}
/*
ARCH: Windows x86/x64
This example flips the stack temporary to a dynamic allocated buffer, using heap portion as stack
If test fails, execution should crash
*/

#include <windows.h>
#include <stdio.h>

size_t old_esp = 0;
size_t old_ebp = 0;

extern size_t Test64(BYTE** buf);

int suma(int a, int b)
{
	return a + b;
}

int main()
{
	BYTE* heap = (BYTE*)malloc(0x100000);
	memset(heap, 0, 0x100000);
	BYTE* stack_top = heap + 0x100000 - 0x10000;
	int res2 = 0;

#ifndef _M_X64
	_asm {
		push eax
		mov eax, stack_top
		xchg eax, esp
		push eax
		push 1000
		push 2000
		call suma
		mov res2, eax
		add esp, 8
		pop eax
		xchg eax, esp
	}
#else
	//res2 = Test64(&stack_top);
#endif

	printf("s=%d\n", res2);
	printf("PASSED");

	return 0;
}

{% endraw %}