	org 0x7c00
_start:
	mov ax, 0xB800
	mov es, ax
	xor si, si
	xor di, di
	mov cx, msglen
write:
	mov al, [si + msg]
	mov [es: di], al
	inc si
	inc di
	inc di
	loop write
forever:
	jmp forever
msg:
	db "hello, world!!"
msglen	equ $ - msg
times 510-($-$$) db 0
	db 0x55, 0xaa

