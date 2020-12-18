; ------------------------------------------------------------------------------
; second stage bootloader enters 32-bit protected mode
; assemble with nasm

STACK32_TOP EQU 0x1000000		; top of 16MB memory
LOADADDR    EQU 0x8000

; 16 bit functions that run in real mode

	bits 16

	global _start
_start:
	xor	ax, ax
	mov	bx, ax
	mov	cx, ax
	mov	dx, ax
	mov	si, ax
	mov	di, ax
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	bp, ax		; initialize ss:sp and ss:bp
	mov	sp, 0x7c00
	mov	si, greeting
	call	_print
	call	_load_program
	call	_enable_a20_line
	cli			; disable interrupts
	call	_disable_nmi	; disable NMI
	call	_enter_prot_mode
	lgdt	[gdt.record]
	jmp	0x08:start32

; ax, cx, dx  clobbered
_load_program:
	push	es
	push	bx
	mov	ah, 2h		; read sectors from drive
	mov	al, 32		; sector count (16K)
	mov	ch, 0		; cylinder
	mov	dh, 0		; head
	mov	cl, 2		; sector
	mov	dl, 0		; drive a (use 80h for 1st HD)
	mov	bx, LOADADDR << 4
	mov	es, bx
	mov	bx, LOADADDR & 0xFFFF ; start address of main program
	int	13h
	pop	bx
	pop	es
	ret

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
	mov	ebp, 0		; terminate chain of frame pointers
	call	LOADADDR	; it's not expected for main to ever return
				; but just in case:
	cli			;   disable interrupts
.halt	hlt			;   Halt CPU with infinite loop
	jmp	.halt


	section .data

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

greeting:
	db `Entering Protected Mode...\r\n`, 0

