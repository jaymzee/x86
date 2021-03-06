; ------------------------------------------------------------------------------
; show argument count
; Runs on 64-bit Linux only.
; To assemble and run:
;
;     nasm -felf64 hello.asm && ld hello.o && ./a.out
; ------------------------------------------------------------------------------

	default rel

	section	.text
	global	_start
_start:
	pop	rax		; argc
	mov	rbx, rax	; save it in a nonvolatile register
.L1	pop	rdi		; argv[0]
	call	strlen
	mov	rdx, rax	; number of bytes
	mov	rsi, rdi	; argv[0]
	mov	rax, 1		; system call for write
	mov	rdi, 1		; file handle 1 is stdout
	syscall			; invoke operating system to do the write
	mov	rax, 1		; system call for write
	mov	rdi, 1		; file handle 1 is stdout
	lea	rsi, [newline]	; argv[0]
	mov	rdx, 1		; number of bytes
	syscall			; invoke operating system to do the write
	dec	rbx
	jnz	.L1
	mov	rax, 60		; system call for exit
	xor	rdi, rdi	; exit code 0
	syscall			; invoke operating system to exit

	global	strlen
strlen:
	mov	rax, -1
.L1	inc	rax
	cmp	byte [rdi + rax], 0
	jne	.L1
	ret

newline:
	db	10
