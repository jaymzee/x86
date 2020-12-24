%include "cpu64.asm"

	extern GPFaultHandlerM
	extern PageFaultHandlerM
	extern DivbyzeroHandlerM
	extern KeyboardHandlerM
	extern puts

	bits 64
	section .text

	global GPFaultHandler
GPFaultHandler:
	cli
	push	rbp
	mov	rbp, rsp
	sub	rsp, reg.size+8
	savregs	rsp
	savisf	rsp, rbp+10h		; fault rip, rflags, rsp, etc.
	mov	rdi, rsp		; pointer to reg struct
	mov	rsi, [rbp+8h]		; fault error code
	call	GPFaultHandlerM
	mov	rdi, cpu_halted
	call	puts
.halt	cli
	hlt
	jmp	.halt
	mov	rsi, [rsp+reg.rsi]
	mov	rdi, [rsp+reg.rdi]
	mov	rax, [rsp+reg.rax]
	leave
	add	rsp, 8			; pop error code
	iretq

	global PageFaultHandler
PageFaultHandler:
	cli
	push	rbp
	mov	rbp, rsp
	sub	rsp, reg.size+8
	savregs	rsp
	savisf	rsp, rbp+10h		; fault rip, rflags, rsp, etc.
	mov	rdi, rsp		; pointer to regs struct
	mov	rsi, [rbp+8h]		; fault error code
	call	PageFaultHandlerM
	mov	rdi, cpu_halted
	call	puts
.halt	cli
	hlt
	jmp	.halt
	mov	rsi, [rsp+reg.rsi]
	mov	rdi, [rsp+reg.rdi]
	mov	rax, [rsp+reg.rax]
	leave
	iretq

	global DivbyzeroHandler
DivbyzeroHandler:
	cli
	push	rbp
	mov	rbp, rsp
	sub	rsp, reg.size+8
	savregs	rsp
	savisf	rsp, rbp+8		; fault rip, rflags, rsp, etc.
	mov	rdi, rsp		; pointer to regs struct
	call	DivbyzeroHandlerM
	mov	rdi, cpu_halted
	call	puts
.halt	cli
	hlt
	jmp	.halt
	mov	rdi, [rsp+reg.rdi]
	mov	rax, [rsp+reg.rax]
	leave
	iretq

	global KeyboardHandler
KeyboardHandler:
	push	rax
	push	rcx
	push	rdx
	push	rbx		; redundant but maintains 16 byte alignment
	push	rsi
	push	rdi
	push	r8
	push	r9
	push	r10
	push	r11
	mov	al, 0x20
	out	0x20, al	; issue EOI
	call	KeyboardHandlerM
	pop	r11
	pop	r10
	pop	r9
	pop	r8
	pop	rdi
	pop	rsi
	pop	rbx
	pop	rdx
	pop	rcx
	pop	rax
	iretq

	global TimerHandler
TimerHandler:
	push	rax
	mov	al, 0x20
	out	0x20, al		; issue EOI
	inc	dword [timer_count]
	pop	rax
	iretq

	global CauseGPFault
CauseGPFault:
	push	rax
	mov	eax, ds
	push	rax
	mov	ax, 0x42
	mov	ds, ax			; should cause GP Fault
	pop	rax
	mov	ds, eax
	pop	rax
	ret

	global CausePageFault
CausePageFault:
	mov	[0x442], eax		; First page is not mapped to catch
	ret				; null pointer exceptions.

	global CauseDivbyzero
CauseDivbyzero:
	mov	eax, 0
	div	eax
	ret

	section .data

	global timer_count
timer_count:
	dd 0

cpu_halted:
	db "system halted", 0
