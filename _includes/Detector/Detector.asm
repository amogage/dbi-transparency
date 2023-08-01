format COFF

extrn '?Sync@@3EA' as Sync

public ThreadWhichModify as '?ThreadWhichModify@@YAXXZ'
ThreadWhichModify:
	mov byte[Sync], 1					; Synchronize the thread, but too early in order to exploit the FIFO property of the processor cache.
	mov byte[codeToModify + 1], 1		; Transform 'mov eax, 0' into 'mov eax, 1'.
	ret

public ModifiedThread as '?ModifiedThread@@YAHXZ'
ModifiedThread:
	cmp byte[Sync], 0					; Wait for the synchronisation.
	jz ModifiedThread					;
		
	;If a DBI is here, or a debug interrupts the detection will occur.
	;In order to detect VM you should add several CPUID here (depending on your processor).

	codeToModify:						; 0B8h is the opcode for mov eax, imm32, here imm32 is 0.
	db 0B8h, 00h, 00h, 00h, 00h			; mov eax, 0
		
	ret

