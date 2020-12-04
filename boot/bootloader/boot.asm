	org 0x7c00
_start:
	mov ah, 2h	; read sectors from drive
	mov al, 60	; sector count
	mov ch, 0	; cylinder
	mov dh, 0	; head
	mov cl, 2	; sector
	mov dl, 0	; select drive a (80h for 1st hd)
	mov bx, 0x100
	int 13h
	jmp 100h

padding:
times 510-($-$$) db 0
magic:
	db 0x55, 0xaa

