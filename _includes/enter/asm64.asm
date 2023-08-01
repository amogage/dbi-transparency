format MS64 COFF

public Fenter as '?Fenter@@YAXXZ'
Fenter:
	enter 1024, 0
	leave
	ret

public Fretf as '?Fretf@@YAXXZ'
Fretf:
	mov rcx, cs
	pushq rcx
	pushq @f
	nop
	nop
	nop
	retf
@@:
	ret
