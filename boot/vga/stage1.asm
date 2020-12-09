; ---------------------------------------------------------------------
; set video mode to vga and draw some lines

	extern Main

	bits 16

	global _start
_start:
	mov	bp, 0
	call	_main
.L1	jmp .L1			; forever

	global main
_main:
	push	di

	mov	ax, 0013h
	int	10h		; set video mode 13h

	push	10
	push	10
	call	_hLine
	push	10
	push	100
	call	_hLine
	push	10
	push	190
	call	_hLine
	push	greeting
	call	_printStr
	mov	ax, 0xA000
	mov	es, ax
	mov	di, 0
	mov	cx, 8
.L1	mov	byte [es: di + 20*320], 10
	inc	di
	loop	.L1
	call	0:Main

	pop	di
	ret

; void hLine(int16 y, int8 color)
_hLine:
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
	call	_writePixel
	inc	si
	cmp	si, 310
	jle	.L1

	pop	di
	pop	si
	pop	bx
	leave
	retn	4

; void writePixel(int16 x, int16 y, int8 color)
_writePixel:
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
_printStr:
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

_printLine:
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
_printInt:
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
