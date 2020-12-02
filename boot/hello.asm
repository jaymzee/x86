;; assemble with:
;; nasm -o hello.bin hello.asm
	org 0x7c00
_start:
	cli			; disable interrupts
	mov ax, 0xB800		; initialize es data segment for vga text
	mov es, ax
clrscr	mov al, 0		; clrscr character
	mov ah, 7		; clrscr attribute
	mov di, 0		; index into screen memory
	mov cx, 2000		; 80 x 25
.L1	mov [es:di], al		; set character
	inc di
	mov [es:di], ah		; set attribute
	inc di
	loop .L1
	mov si, greeting
	mov di, 0		; index into screen memory
	mov cx, greetingL
.L2	mov al, [si]		; get next char from greeting
	mov [es:di], al		; write to screen memory
	inc si			; increment index for next char
	add di, 2		; increment index for screen memory
	loop .L2
forever	jmp forever

greeting:
	db "hello, world!!"
greetingL equ $ - greeting
padding:
times 510-($-$$) db 0
magic:
	db 0x55, 0xaa

