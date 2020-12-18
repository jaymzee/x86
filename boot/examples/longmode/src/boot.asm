; ------------------------------------------------------------------------------
; second stage bootloader enters long mode
; paging is required in long mode
; only the first 2MB is identity mapped to show proof of concept
; assemble with nasm

STACK64_TOP equ 0x1000000	; top of 16MB memory
LOADADDR    equ 0x10000		; address to load program into


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
	call	_init_page_tables
	cli			; disable interrupts
	call	_disable_nmi	; disable NMI
	call	_enter_long_mode
	lgdt	[gdt.record]
	jmp	0x08:start64

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
	mov	bx, LOADADDR >> 4
	mov	es, bx
	mov	bx, LOADADDR & 0xFFFF ; es:bx start address of main program
	int	13h
	pop	bx
	pop	es
	ret

%include "bios.asm"
%include "cpumode.asm"

; initialize page tables
; eax, ecx, edx  clobbered
_init_page_tables:
	push	edi
	mov	edi, 0x2000	; start of page tables
	mov	cr3, edi
	xor	eax, eax
	cld
	mov	ecx, 512 * 2 * 3
	rep	stosd		; clear page tables
	mov	edi, cr3
	mov	eax, 0x1000	; eax = page size
	mov	edx, edi
	add	edx, 0x1003	; edx -> next page table
	mov	[edi], edx	; PML4T[0] -> PDPT
	add	edi, eax
	add	edx, eax
	mov	[edi], edx	; PDPT[0] -> PDT
	add	edi, eax
	add	edx, 0x4000
	mov	ecx, 8
.pdt	mov	[edi], edx	; PDT[n] -> nth PT
	add	edx, eax
	add	edi, 8
	loop	.pdt
	mov	edi, 0x8000
	mov	edx, 3		; R/W and Present
	mov	ecx, 4096	; identity map first 16MB
.fillpt	mov	[edi], edx	; PT[n] = n*4096 + 3
	add	edx, 0x1000
	add	edi, 8
	loop	.fillpt
	pop	edi
	ret


; 64 bit code that run in long mode

	bits 64

	default abs

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
	mov	ebp, 0		; terminate chain of frame pointers
	mov	rax, LOADADDR
	call	rax		; call main()
				; not expect for it to return but if it does:
	cli			;   disable interrupts
.halt	hlt			;   Halt CPU with infinite loop
	jmp	.halt

	section .data
	align 8
gdt:
	dq 0			; First entry is always the Null Descriptor
	; 4gb flat r/executable code descriptor
.code	dw 0xFFFF		; limit 0:15
	dw 0			; base 0:15
	db 0			; base 16:23
	db 0b10011010		; access P DPL S, flags Ex DC R Ac
	db 0b10101111		; flags Gr Sz L, Limit 16:19
	db 0			; base 24:31
	; 4gb flat r/w data descriptor
.data	dw 0xFFFF		; limit 0:15
	dw 0			; base 0:15
	db 0			; base 16:23
	db 0b10010010		; access P DPL S, flags Ex DC W Ac
	db 0b10101111		; flags Gr Sz L, Limit 16:19
	db 0			; base 24:31
.record	dw $ - gdt - 1		; size
	dd gdt			; offset

greeting:
	db `Entering long mode...\r\n`, 0

