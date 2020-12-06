; floppy disk boot block

	bits 16
	global _start
_start:
	mov	ah, 2h	; read sectors from drive
	mov	al, 59	; sector count
	mov	ch, 0	; cylinder
	mov	dh, 0	; head
	mov	cl, 2	; sector
	mov	dl, 0	; drive a (use 80h for 1st HD)
	mov	bx, 500h	; start address of next boot loader
	int	13h
	jmp	500h	; jump to 2nd level boot loader
	times 510-($-$$) db 0
	db 0x55, 0xaa
