;; assemble with:
;; nasm -o hello.bin hello.asm

	bits 16
	org 0x7c00
_start:
	mov	ax, 0xB800	; initialize es data segment for vga text
	mov	es, ax
	mov	al, 0		; clrscr character
	mov	ah, 7		; clrscr attribute
	mov	di, 0		; index into screen memory
	mov	cx, 2000	; 80 x 25
.L1	mov	[es:di], al	; set character
	inc	di
	mov	[es:di], ah	; set attribute
	inc	di
	loop	.L1
	mov	si, greeting
	mov	di, 0		; index into screen memory
	mov	cx, greetingL
.L2	movsb			; copy greeting to screen memory
	inc	di		; increment screen memory index again
	loop	.L2

forever jmp forever

greeting:
	db "hello, world!!"
greetingL equ $ - greeting

	times 510-($-$$) db 0
	db 0x55, 0xaa		; block signature
