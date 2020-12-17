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
	call	_print
	call	_enable_a20_line
	cli			; disable interrupts
	call	_disable_nmi	; disable NMI
	call	_enter_prot_mode
	lgdt	[gdt.record]
	jmp	0x08:start32

%include "bios.asm"
%include "cpumode.asm"



; 32 bit code that will run in 32-bit protected mode

	bits 32

	section .text
	align 4
start32:
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
gdt:
	dq 0		; First entry is always the Null Descriptor
.code	equ $ - gdt
	; 4gb flat read/executable code descriptor
	dw 0xFFFF	; limit 0:15
	dw 0		; base 0:15
	db 0		; base 16:23
	db 0b10011010	; access P GPL S, Type Ex DC R Ac
	db 0b11001111	; flags Gr Sz L, Limit 16:19
	db 0		; base 24:31
.data	equ $ - gdt
	; 4gb flat read/write data descriptor
	dw 0xFFFF	; limit 0:15
	dw 0		; base 0:15
	db 0		; base 16:23
	db 0b10010010	; access P GPL S Type Ex DC W Ac
	db 0b11001111	; flags Gr Sz L, Limit 16:19
	db 0		; base 24:31
.record	dw $ - gdt - 1	; size
	dd gdt		; offset

