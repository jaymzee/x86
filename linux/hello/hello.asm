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
	section	.text

	global	_start
_start	mov	rax, 1		; system call for write
	mov	rdi, 1		; file handle 1 is stdout
	lea	rsi, [mesg]	; address of string to output
	mov	rdx, mesglen	; number of bytes
	syscall			; invoke operating system to do the write
	mov	rax, 60		; system call for exit
	xor	rdi, rdi	; exit code 0
	syscall			; invoke operating system to exit

	section	.rodata
mesg	db "Hello, World!",10
mesglen	equ $ - mesg
