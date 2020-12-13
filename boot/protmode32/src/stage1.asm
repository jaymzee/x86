; ------------------------------------------------------------------------------
; second stage bootloader enters 32-bit protected mode
; assemble with nasm

STACK32_TOP EQU 0x1000000		; top of 16MB memory
VIDEOMEM    EQU 0x0b8000

	extern Main

; 16 bit functions that run in real mode

	bits 16

	global _start
_start:
	mov	si, greeting1
	call	bios_print
	jmp	enter_prot_mode

; print to screen using calls to BIOS INT 10h
;   si  pointer to null terminated string
;   ax  clobbered
bios_print:
	push	bx
	cld
	mov	bx, 0x000f		; page number (and color in gfx mode)
	mov	ah, 0Eh			; teletype output
.L1	lodsb
	test	al, al
	je	.break
	int	10h
	jmp	.L1
.break	pop	bx
	ret


enter_prot_mode:
	call	a20_on		; Enable A20 gate
	cli			; disable interrupts
	lgdt	[gdtr]		; Load gdt
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax	; Set protected mode flag
	jmp	0x08:start_32

; Enable A20
;   uses fast method as proof of concept and may not work on all hardware
;   ax  clobbered
a20_on:
	cli			; disable interrupts
	in	al, 0x92	; Read System Control Port A
	test	al, 0x02	; Test current a20 value (bit 1)
	jnz	.skip		; If already 1 skip a20 enable
	or	al, 0x02	; Set a20 bit (bit 1) to 1
	and	al, 0xfe	; Always write a zero to bit 0 to avoid
				;     a fast reset into real mode
	out	0x92, al        ; Enable a20
.skip	sti			; reenable interrupts
	ret


; Code that will run in 32-bit protected mode

	bits 32

	section .code
	align 4
start_32:
	cld			; Direction flag forward
	mov	eax, 0x10	; 0x10 is flat selector for data
	mov	ds, eax
	mov	es, eax
	mov	fs, eax
	mov	gs, eax
	mov	ss, eax
	mov	esp,STACK32_TOP ; Should set ESP to a usable memory location
				; Stack will be grow down from this location

	; print success message to display
	mov	ah, 0x57	; Attribute white on magenta
	mov	esi, greeting2	; ESI = address of string to print
	mov	edi, VIDEOMEM	; EDI = base address of video memory
	call	write_screen

	mov	ebp, 0		; terminate chain of frame pointers
	call	Main		; it's not expected for Main to ever return
				; but just in case:
	cli			;   disable interrupts
.halt	hlt			;   Halt CPU with infinite loop
	jmp	.halt

; display text and attribute at a location on the screen
;   edi - null terminated string to display
;   esi - screen memory address to write the string to
;    ah - screen attribute to use for each character
;   all other registers are preserved
write_screen:
	push	ecx
	xor	ecx, ecx	; ECX = 0 current video offset
	jmp	.L1_ent
.L1	mov	[edi+ecx*2], ax	; Copy attr and character to display
	inc	ecx		; Next word position
.L1_ent mov	al, [esi+ecx]	; Get next character to print
	test	al, al
	jnz	.L1		; If it's not NUL continue
	pop	ecx
	ret


	section .data
greeting1:
	db `Entering Protected Mode\r\n`, 0
greeting2:
	db "Protected Mode entered successfully - console on serial 0", 0

	align 8
gdtr:
	dw gdt_end-gdt_start - 1
	dd gdt_start

	align 8
gdt_start:
	; First entry is always the Null Descriptor
	dq 0
gdt_code:
	; 4gb flat r/w/executable code descriptor
	dw 0xFFFF	; limit low
	dw 0		; base low
	db 0		; base middle
	db 0b10011010	; access
	db 0b11001111	; granularity
	db 0		; base high
gdt_data:
	; 4gb flat r/w data descriptor
	dw 0xFFFF	; limit low
	dw 0		; base low
	db 0		; base middle
	db 0b10010010	; access
	db 0b11001111	; granularity
	db 0		; base high
gdt_end:
