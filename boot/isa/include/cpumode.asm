; 16-bit functions that run in real mode
; you should include this file

; enter long mode (64-bit protected mode)
;   eax, ecx  clobbered
	global _enter_long_mode
_enter_long_mode:
.en_pae	mov	eax, cr4
	or	eax, 1 << 5	; enable PAE mode paging
	mov	cr4, eax
.set_lm	mov	ecx, 0xC0000080 ; EFER MSR
	rdmsr
	or	eax, 1 << 8	; set LM-bit in the EFER MSR
	wrmsr
.en_pr	mov	eax, cr0
	or	eax, 1 << 31 | 1; enable paging and protected mode
	mov	cr0, eax
	ret

; enter 32-bit protected mode
;   eax  clobbered
_enter_prot_mode:
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax	; Set protected mode flag
	ret

; Enable A20
;   uses fast method as proof of concept and may not work on all hardware
;   ax  clobbered
	global _enable_a20_line
_enable_a20_line:
	cli			; disable interrupts
	in	al, 0x92	; Read System Control Port A
	test	al, 0x02	; Test current a20 value (bit 1)
	jnz	.skip		; If already 1 skip a20 enable
	or	al, 0x02	; Set a20 bit (bit 1) to 1
	and	al, 0xfe	; Always write a zero to bit 0 to avoid
				;     a fast reset into real mode
	out	0x92, al	; Enable a20
.skip	sti			; reenable interrupts
	ret

; disable NMI
;   al  clobbered
_disable_nmi:
	in	al, 0x70
	or	al, 0x80
	out	0x70, al
	ret

; enable NMI
;   al  clobbered
_enable_nmi:
	in	al, 0x70
	and	al, 0x7f
	out	0x70, al
	ret
