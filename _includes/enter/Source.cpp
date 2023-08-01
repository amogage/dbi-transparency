{% raw %}
/*
ARCH: Windows x86/x64
Tis example uses enter instruction.
From documentation, tis instruction makes trubles to Valgrind
Credits to:
https://github.com/zhechkoz/PwIN.git
https://link.springer.com/chapter/10.1007/978-3-319-99073-6_18
*/

#include <windows.h>
#include <stdio.h>

#ifdef _M_X64
extern void Fenter();
#endif

int main(void)
{
#ifndef _M_X64
	//function stackframe
	_asm {
		enter 1024,0
		leave
	}
#else
	Fenter();
#endif

	printf("PASSED");

	return 0;
}
{% endraw %}