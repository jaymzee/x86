; ----------------------------------------------------------------------
; set video mode to vga and draw some lines

	extern square

	bits 16

	global _start
_start:
	mov	bp, 0
	call	main
.L1	jmp .L1			; forever


	global main
main:
	push	regsp
	call	printString
	push	sp
	call	printInt
	call	printLine

	push	greeting
	call	printString

	mov	eax, 3
	push	eax
	call	0:square
	pop	ecx
	push	ax
	call	printInt
	call	printLine

	push	regsp
	call	printString
	push	sp
	call	printInt
	call	printLine

	ret

; void writeString(char *str)
printString:
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

printLine:
	push	bx
	mov	bx, 0x0000	; page number (and color in gfx mode)
	mov	ah, 0Eh		; teletype output
	mov	al, 13
	int	10h
	mov	al, 10
	int	10h
	pop	bx
	ret


; void printInt(int16 x)
printInt:
	push	bp
	mov	bp, sp
	push	bx

	mov	dx, [bp + 4]	; x
	mov	cx, 4		; len
	mov	bx, 0x000f	; page number (and color in gfx mode)
	mov	ah, 0Eh		; teletype output
.L1	rol	dx, 4
	mov	al, dl
	and	al, 0fh
	cmp	al, 10
	jl	.L2
	add	al, 7
.L2	add	al, 0x30
	int	10h
	loop	.L1

	pop	bx
	leave
	retn	2

greeting:
	db `square(3) = `, 0
regsp:
	db `SP = 0x`, 0
