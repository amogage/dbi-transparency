format MS64 COFF

suma2:
	mov rax, qword [rsp + 8]
	add rax, qword [rsp + 16]
	ret

public Test64 as '?Test64@@YA_KPEAPEAE@Z'
Test64:
	mov rax, qword [rcx]
	xchg rax, rsp
	push rax
	push 1000
	push 2000
	call suma2
	mov rbx, rax
	add rsp, 16
	pop rax
	xchg rax, rsp
	mov rax, rbx
	ret
