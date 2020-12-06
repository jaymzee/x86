; ---------------------------------------------------------------------
; set video mode to vga and draw some lines

	bits 16
	global _start
_start:
	mov	bp, 0
	call	main
.L1	jmp .L1			; forever

	global main
main:
	mov	ax, 0013h
	int	10h		; set video mode 13h

	mov	ax, 10
	push	ax
	mov	ax, 10
	push	ax
	call	hLine
	mov	ax, 10
	push	ax
	mov	ax, 100
	push	ax
	call	hLine
	mov	ax, 10
	push	ax
	mov	ax, 190
	push	ax
	call	hLine
	push	greeting
	call	printString
	ret

; void hLine(int16 y, int8 color)
hLine:
	push	bp
	mov	bp, sp
	push	bx
	push	si
	push	di

	mov	si, 10		; x = 10
	mov	di, [bp + 4]	; y
	mov	bx, [bp + 6]	; color
.L1	push	bx		; color
	push	di		; y
	push	si		; x
	call	writePixel
	inc	si
	cmp	si, 310
	jle	.L1

	pop	di
	pop	si
	pop	bx
	leave
	retn	4

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

; void writeString(char *str)
printString:
	push	bp
	mov	bp, sp
	push	bx
	push	si

	mov	si, [bp + 4]	; str
	mov	bx, 0x000f	; page number (and color in gfx mode)
	mov	ah, 0Eh		; teletype output
.L1	mov	al, [si]
	cmp	al, 0
	je	.done
	int	10h
	inc	si
	loop	.L1

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
	db `Hello, world!\r\n`, 0
