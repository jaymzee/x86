; basic i/o using the BIOS ;)
; you should include this file.
; it's intended to be used in 16-bit real mode code

; print to screen using calls to BIOS INT 10h
;   si  pointer to null terminated string
;   ax  clobbered
	global _print
_print:
	push	bx
	cld
	mov	bx, 0x000f	; page number (and color in gfx mode)
	mov	ah, 0Eh		; teletype output
.L1	lodsb
	test	al, al
	je	.break
	int	10h
	jmp	.L1
.break	pop	bx
	ret
