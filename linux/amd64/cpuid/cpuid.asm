	default rel
	section .text

	global _start
_start:
	xor	eax, eax
	cpuid				; EBX, EDX, ECX will contain mfr string
	lea	rsi, [buf]
	mov	[rsi], ebx
	mov	[rsi+4], edx
	mov	[rsi+8], ecx
	mov	eax, 1			; write
	mov	edi, 1			; stdout
	mov	edx, 13
	syscall
	mov	eax, 60
	xor	edi, edi
	syscall

	section .data
buf:
	times 12 db 0
	db	10, 0

