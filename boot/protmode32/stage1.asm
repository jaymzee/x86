; ------------------------------------------------------------------------------
; second stage bootloader enter protected mode
; assemble with nasm

STACK32_TOP EQU 0x200000		; top of 2MB memory
CODE32_REL  EQU 0x110000
VIDEOMEM    EQU 0x0b8000

	extern Main

	bits 16

	global _start
_start:
	push	greeting
	call	_bios_print
	jmp	enter_prot_mode

; void bios_print(char *str)		; print to screen using BIOS INT 10h
_bios_print:
	push	bp
	mov	bp, sp
	push	bx
	push	si

	mov	si, [bp + 4]		; str
	mov	bx, 0x000f		; page number (and color in gfx mode)
	mov	ah, 0Eh			; teletype output
.L1	lodsb
	test	al, al
	je	.done
	int	10h
	jmp	.L1

.done	pop	si
	pop	bx
	leave
	retn	2


enter_prot_mode:
	call	a20_on		; Enable A20 gate (uses Fast method as proof of concept)
	cli			; disable interrupts

	; Compute linear address of label gdt_start
	; Using (segment << 4) + offset
	mov	eax, cs		; EAX = CS
	shl	eax, 4		; EAX = (CS << 4)
	mov	ebx, eax	; Make a copy of (CS << 4)
	add	[gdtr+2], eax	; Add base linear address to gdt_start address
				;     in the gdtr
	lgdt	[gdtr]		; Load gdt

	; Compute linear address of label code_32bit
	; Using (segment << 4) + offset
	add	ebx, code_32bit	; EBX = (CS << 4) + code_32bit

	push	dword 0x08	; CS Selector
	push	ebx		; Linear offset of code_32bit
	mov	bp, sp		; m16:32 address on top of stack, point BP to it

	mov	eax,cr0
	or	eax,1
	mov	cr0,eax		; Set protected mode flag

	jmp	dword far [bp]	; Indirect m16:32 FAR jmp with
				;    m16:32 constructed at top of stack
				;    DWORD allows us to use a 32-bit offset in 16-bit code

; 16-bit functions that run in real mode

; Check if protected mode is enabled, effectively checkign if we are
; in in a VM8086 task. Set ZF to 0 if in protected mode
check_pmode:
	smsw	ax		; Get lower 16 bits of control register in AX
	test	ax, 0x1		; Test the PE bit (bit 0) and set ZF flag accordingly
	ret

; Enable a20 (fast method). This may not work on all hardware
a20_on:
	cli			; disable interrupts
	in	al, 0x92	; Read System Control Port A
	test	al, 0x02	; Test current a20 value (bit 1)
	jnz	.skip		; If already 1 skip a20 enable
	or	al, 0x02	; Set a20 bit (bit 1) to 1
	and	al, 0xfe	; Always write a zero to bit 0 to avoid
				;     a fast reset into real mode
	out	0x92, al        ; Enable a20
.skip:
	sti			; reenable interrupts
	ret

greeting:
	db `Entering Protected Mode\r\n`, 0


	align 4
	section .data
gdtr:
	dw gdt_end-gdt_start-1
	dd gdt_start

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


	dq 0, 0		; some padding


; Code that will run in 32-bit protected mode
; Align code to 4 byte boundary. code_32bit label is
; relative to the origin point 100h

	align 4
	bits 32
code_32bit:

; Set virtual memory address of pm code/data to CODE32_REL
start_32:
	cld			; Direction flag forward
	mov	eax,0x10	; 0x10 is flat selector for data
	mov	ds,eax
	mov	es,eax
	mov	fs,eax
	mov	gs,eax
	mov	ss,eax
	mov	esp,STACK32_TOP	; Should set ESP to a usable memory location
				; Stack will be grow down from this location

	jmp	0x08:.relentry	; Absolute jump to relocated code

.relentry:
	mov	ah, 0x57	; Attribute white on magenta

	; Print a string to display
	mov	esi,pmode_str	; ESI = address of string to print
	mov	edi,VIDEOMEM	; EDI = base address of video memory
	call	write_screen
	mov	ebp, 0		; terminate chain of frame pointers
	call	Main		; not expected for Main to return

	cli			; disable interrupts
.forever:
	hlt			; Halt CPU with infinite loop
	jmp	.forever

write_screen:
	push	ecx
	xor	ecx, ecx	; ECX = 0 current video offset
	jmp	.loopentry
.L1:
	mov	[edi+ecx*2], ax	; Copy attr and character to display
	inc	ecx		; Next word position
.loopentry:
	mov	al, [esi+ecx]	; Get next character to print
	test	al, al
	jnz	.L1		; If it's not NUL continue
	pop	ecx
	ret

pmode_str:
	db "Protected Mode entered successfully - console on serial 0", 0

; Number of DWORDS that the protected mode section takes up (rounded up)
PMSIZE_LONG equ ($-$$+3)>>2
