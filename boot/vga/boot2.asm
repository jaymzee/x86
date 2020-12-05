; set video mode to vga and draw some lines
;
	bits 16

	org 0x500
_start:
	mov	bp, sp
	call	main
.L1	jmp .L1			; forever

main:
	push	bp
	mov	bp, sp
	sub	sp, 16		; local alloc
	push	bx

	mov	ax, greetingL
	push	ax
	push	greeting
	call	printString

	mov	ax, 0013h
	int	10h		; set video mode 13h

	push	word greeting2L
	push	greeting2
	call	printString

	push	sp
	call	printInt
	call	printLine

	mov	word [bp+2], 10	; x = 10
	mov	word [bp+4], 10	; y = 10
.L1	push	10		; color
	push	word [bp+4]	; y
	push	word [bp+2]	; x
	call	writePixel
	inc	word [bp+2]
	cmp	word [bp+2], 310
	jle	.L1

	push	sp
	call	printInt
	call	printLine

	pop	bx
	leave
	ret

; void writePixel(int16 x, int16 y, int8 color)
writePixel:
	push	bp
	mov	bp, sp
	push	bx

	mov	cx, [bp + 4]	; x
	mov	dx, [bp + 6]	; y
	mov	ax, [bp + 8]	; color
	mov	bh, 0		; page number
	mov	ah, 0ch		; write pixel
	int	10h

	pop	bx
	leave
	retn	6

; void writeString(char *str, int16 len)
printString:
	push	bp
	mov	bp, sp
	push	bx
	push	si

	mov	si, [bp + 4]	; str
	mov	cx, [bp + 6]	; len
	mov	bx, 0x000f	; page number (and color in gfx mode)
	mov	ah, 0Eh		; teletype output
.L1	mov	al, [si]
	int	10h
	inc	si
	loop	.L1

	pop	si
	pop	bx
	leave
	retn	4

printLine:
	push	bx
	mov	bx, 0x0000	; page number (and color in gfx mode)
	mov	ah, 0Eh		; teletype output
	mov	al, 13
	int	10h
	mov	al, 10
	int	10h
	pop	bx
	retn	4


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
	db `Entering VGA mode 13h\r\n`
greetingL equ $ - greeting
greeting2:
	db `VGA mode 13h\r\n`
greeting2L equ $ - greeting2
