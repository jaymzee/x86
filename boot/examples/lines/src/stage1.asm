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
	mov	ax, 0x0013
	int	10h		; swtich to vga mode 13h
	call	_enable_a20_line
	cli			; disable interrupts
	call	_enter_prot_mode
	lgdt	[gdtr]
	jmp	0x08:start32

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
	mov	ebp, 0		; terminate chain of frame pointers
	call	Main		; it's not expected for Main to ever return
				; but just in case:
	cli			;   disable interrupts
.halt	hlt			;   Halt CPU with infinite loop
	jmp	.halt


	section .data

	align 8
gdtr:
.size	dw gdt.end - gdt	; size of descriptor table - 1
.offset	dd gdt

	align 8
gdt:
	dq 0			; First entry is always the Null Descriptor
	; 4gb flat read/executable code descriptor
.code	dw 0xFFFF		; limit 0:15
	dw 0			; base 0:15
	db 0			; base 16:23
	db 0b10011010		; access P GPL S, Type Ex DC R Ac
	db 0b11001111		; flags Gr Sz L, Limit 16:19
	db 0			; base 24:31
	; 4gb flat read/write data descriptor
.data	dw 0xFFFF		; limit 0:15
	dw 0			; base 0:15
	db 0			; base 16:23
	db 0b10010010		; access P GPL S Type Ex DC W Ac
	db 0b11001111		; flags Gr Sz L, Limit 16:19
.end	db 0			; base 24:31
