STDOUT	equ	1
WRITE	equ	4
EXIT	equ	1

	default	rel
	section .text

	global _start
_start:
	mov	eax, WRITE
	mov	ebx, STDOUT
	mov	ecx, mesg
	mov	edx, mesglen
	int	0x80

	mov	eax, EXIT
	mov	ebx, 42		; exit code
	int	0x80

mesg	db "Hello, World!", 10
mesglen equ $ - mesg
