	bits 16
	global _start
_start:
	mov si, greeting
	mov cx, greetingL
	mov ah, 0Eh		; teletype output
	mov bx, 0		; page number
.L1	mov al, [si]
	int 10h
	inc si
	loop  .L1
forever	jmp forever

greeting:
	db "hello, world!!", 13, 10, "Hi!"
greetingL equ $ - greeting
