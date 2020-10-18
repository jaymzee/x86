; ------------------------------------------------------------------------------
; show argument count
; Runs on 64-bit Linux only.
; To assemble and run:
;
;     nasm -felf64 hello.asm && ld hello.o && ./a.out
; ------------------------------------------------------------------------------

	default rel
	global	_start

	section	.text
_start	pop	rax
	mov	[argc], eax
	add	[mesg], al

	pop	rsi		; argv[0]
	mov	rax, 1		; system call for write
	mov	rdi, 1		; file handle 1 is stdout
	mov	rdx, 4		; number of bytes
	syscall			; invoke operating system to do the write

	mov	rax, 1		; system call for write
	mov	rdi, 1		; file handle 1 is stdout
	lea	rsi, [mesg]	; address of string to output
	mov	rdx, mesglen	; number of bytes
	syscall			; invoke operating system to do the write

	mov	rax, 60		; system call for exit
	xor	rdi, rdi	; exit code 0
	syscall			; invoke operating system to exit

	section	.data
argc	dd	0
mesg	db	"0",10
mesglen	equ $ - mesg
