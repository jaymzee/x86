; ------------------------------------------------------------------------------
; Writes "Hello, World" to the console using only system calls.
; Runs on 64-bit Linux only.
; To assemble and run:
;
;     nasm -felf64 hello.asm && ld hello.o && ./a.out
; to add debug info assemble with:
;     nasm -felf64 -Fdwarf hello.asm
; ------------------------------------------------------------------------------

	default rel
	global	_start

	section	.text
_start:
	vmovsd	xmm0, [x]
	vmovsd	xmm1, [y]
	vmulpd	ymm0, ymm1
	vmovsd	[z], xmm0
	mov	rax, 60		; system call for exit
	xor	rdi, rdi	; exit code 0
	syscall			; invoke operating system to exit

	section	.data
x:	dq	3.14
y:	dq	2.7
z:	dq	0.0
