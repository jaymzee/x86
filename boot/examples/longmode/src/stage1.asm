; ------------------------------------------------------------------------------
; second stage bootloader enters long mode
; paging is required in long mode
; only the first 2MB is identity mapped to show proof of concept
; assemble with nasm

STACK64_TOP EQU 0x200000	; top of 2MB memory
VIDEOMEM    EQU 0x0b8000

	extern Main

; 16 bit functions that run in real mode

	bits 16

	global _start
_start:
	mov	si, greeting1
	call	_print
	call	_enable_a20_line
	call	_init_page_tables
	cli			; disable interrupts
	call	_enter_long_mode
	lgdt	[gdtr]
	jmp	0x08:start64

%include "bios.asm"
%include "cpumode.asm"

; initialize page tables
; eax, ecx, edx  clobbered
_init_page_tables:
	push	edi
.clr_pd	mov	edi, 0x1000
	mov	cr3, edi
	xor	eax, eax
	mov	ecx, 4096
	rep	stosd		; clear page directory tables
.set_pd	mov	edi, cr3
	mov	eax, 0x1000
	mov	ecx, 0x2003
	mov	[edi], ecx	; PML4T[0] -> PDPT
	add	edi, eax
	add	ecx, eax
	mov	[edi], ecx	; PDPT[0] -> PDT
	add	edi, eax
	add	ecx, eax
	mov	[edi], ecx	; PDT[0] -> PT
	add	edi, eax
	add	ecx, eax
.id_map cld
	mov	edx, 3		; R/W and Present
	mov	ecx, 512	; identity map first 2MB
.fillpt	mov	[edi], edx	; PT[n] = n*4096 + 3
	add	edx, 0x1000
	add	edi, 8
	loop	.fillpt
	pop	edi
	ret


; 64 bit code that run in long mode

	bits 64

	section .text
	align 8
start64:
	cld			; Direction flag forward
	mov	eax, 0x10	; 0x10 is flat selector for data
	mov	ds, eax
	mov	es, eax
	mov	fs, eax
	mov	gs, eax
	mov	ss, eax
	mov	esp,STACK64_TOP ; Should set ESP to a usable memory location
				; Stack will be grow down from this location

	mov	ah, 0x57	; Attribute white on magenta
	mov	esi, greeting2	; ESI = address of string to print
	mov	edi, VIDEOMEM	; EDI = base address of video memory
	call	write_screen	; print success message to display

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
	push	rcx
	xor	ecx, ecx	; ECX = 0 current video offset
	jmp	.L1_ent
.L1	mov	[edi+ecx*2], ax	; Copy attr and character to display
	inc	ecx		; Next word position
.L1_ent mov	al, [esi+ecx]	; Get next character to print
	test	al, al
	jnz	.L1		; If it's not NUL continue
	pop	rcx
	ret


	section .data

greeting1:
	db `Entering long mode...\r\n`, 0
greeting2:
	db "long mode (x64) entered successfully - console on serial 0", 0

	align 8
gdtr:
	dw gdt_end - gdt_start - 1
	dd gdt_start

	align 8
gdt_start:
	; First entry is always the Null Descriptor
	dq 0
gdt_code:
	; 4gb flat r/executable code descriptor
	dw 0xFFFF	; limit 0:15
	dw 0		; base 0:15
	db 0		; base 16:23
	db 0b10011010	; access P DPL S, flags Ex DC R Ac
	db 0b10101111	; flags Gr Sz L, Limit 16:19
	db 0		; base 24:31
gdt_data:
	; 4gb flat r/w data descriptor
	dw 0xFFFF	; limit 0:15
	dw 0		; base 0:15
	db 0		; base 16:23
	db 0b10010010	; access P DPL S, flags Ex DC W Ac
	db 0b10101111	; flags Gr Sz L, Limit 16:19
	db 0		; base 24:31
gdt_end:
