	extern Main

	bits 16

	global _start
_start:
	push	greeting
	call	_print
	call	0:Main
.L1	jmp .L1			; forever

; void print(char *str)	;	; print to screen using BIOS
_print:
	push	bp
	mov	bp, sp
	push	bx
	push	si

	mov	si, [bp + 4]	; str
	mov	bx, 0x000f	; page number (and color in gfx mode)
	mov	ah, 0Eh		; teletype output
.L1	lodsb
	cmp	al, 0
	je	.done
	int	10h
	jmp	.L1

.done	pop	si
	pop	bx
	leave
	retn	2

greeting:
	db `console on serial 0\r\n`, 0
