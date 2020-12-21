; 16-bit functions and macros that run in real mode

; enter 32-bit protected mode
;   eax  clobbered
%macro stprmd 0
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax	; Set protected mode flag
%endmacro

; disable NMI
;   al  clobbered
%macro clnmi 0
	in	al, 0x70
	or	al, 0x80
	out	0x70, al
%endmacro

; enable NMI
;   al  clobbered
%macro stnmi 0
	in	al, 0x70
	and	al, 0x7f
	out	0x70, al
%endmacro

; Enable A20 using System Control Port A
;   uses fast method as proof of concept and may not work on all hardware
;   ax  clobbered
%macro a20ensc 0
	cli			; disable interrupts
	in	al, 0x92	; Read System Control Port A
	test	al, 0x02	; Test current a20 value (bit 1)
	jnz	%%skip		; If already 1 skip a20 enable
	or	al, 0x02	; Set a20 bit (bit 1) to 1
	and	al, 0xfe	; Always write a zero to bit 0 to avoid
				;     a fast reset into real mode
	out	0x92, al	; Enable a20
%%skip	sti			; reenable interrupts
%endmacro

; Enable A20 using BIOS method
%macro a20enbio 0
	mov	ax,2401h
	int	15h
%endmacro

; copy the gdt so that the memory used by the bootloader
; can be reclaimed for something else
;   ds:si  offset to copy the gdt from
;   es:di  offset to copy the gdt to
%macro gdtcpy 1
	push	bx
	mov	bx,di
	mov	cx,%1
	add	bx,cx
	mov	word [bx],%1-1	; write GDTR limit
	mov	word [bx+2],di	; write GDTR offset bits 15..0
	mov	word [bx+4],0	; write GDTR offset bits 31..16
	rep	movsb
	pop	bx
%endmacro

; enter long mode (64-bit protected mode)
;   eax, ecx  clobbered
%macro stlgmd 0
.en_pae	mov	eax, cr4
	or	eax, 1 << 5	; enable PAE mode paging
	mov	cr4, eax
.set_lm	mov	ecx, 0xC0000080 ; EFER MSR
	rdmsr
	or	eax, 1 << 8	; set LM-bit in the EFER MSR
	wrmsr
.en_ppr	mov	eax, cr0
	or	eax, 1 << 31 | 1; enable paging and protected mode
	mov	cr0, eax
%endmacro
