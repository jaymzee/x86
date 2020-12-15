; ------------------------------------------------------------------------------
; second stage bootloader enters long mode
; paging is required in long mode
; only the first 2MB is identity mapped to show proof of concept
; assemble with nasm

STACK32_TOP EQU 0x200000		; top of 2MB memory
VIDEOMEM    EQU 0x0b8000

	extern Main

; 16 bit functions that run in real mode

	bits 16

	global _start
_start:
	mov	si, greeting1
	call	bios_print
	jmp	enter_long_mode

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


enter_long_mode:
	call	enable_a20	; Enable A20 gate
	cli			; disable interrupts
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
	mov	ebx, 3
	mov	ecx, 512	; identity map first 2MB
.fillpt	mov	[edi], ebx	; PT[n] = n*4096 + 3
	add	ebx, 0x1000
	add	edi, 8
	loop	.fillpt
.en_pae	mov	eax, cr4
	or	eax, 1 << 5	; enable PAE-paging
	mov	cr4, eax
.set_lm	mov	ecx, 0xC0000080 ; EFER MSR
	rdmsr
	or	eax, 1 << 8	; set LM-bit in the EFER MSR
	wrmsr
.en_pr	mov	eax, cr0
	or	eax, 1 << 31 | 1; enable paging and protected mode
	mov	cr0, eax
	lgdt	[gdtr]		; Load gdt
	jmp	0x08:start64

; Enable A20
;   uses fast method as proof of concept and may not work on all hardware
;   ax  clobbered
enable_a20:
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
	mov	esp,STACK32_TOP ; Should set ESP to a usable memory location
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
