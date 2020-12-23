%include "cpu64.asm"

	extern GPFaultHandlerM
	extern PageFaultHandlerM
	extern KeyboardHandlerM
	extern println

	bits 64
	section .text

	global GPFaultHandler
GPFaultHandler:
	cli
	sub	rsp, reg.size
	savregs	rsp
	mov	rax, [rsp+reg.size+8]	; fault rip
	mov	[rsp+reg.rip], rax
	mov	ax,[rsp+reg.size+10h]	; fault cs
	mov	[rsp+reg.cs], ax
	mov	rax, [rsp+reg.size+18h]	; fault rflags
	mov	[rsp+reg.rflags], rax
	mov	rax, [rsp+reg.size+20h]	; fault rsp
	mov	[rsp+reg.rsp], rax
	mov	ax, [rsp+reg.size+28h]	; fault ss
	mov	[rsp+reg.ss], ax
	mov	rdi, [rsp+reg.size]	; fault error code
	mov	rsi, rsp		; pointer to reg struct
	call	GPFaultHandlerM
	mov	rdi, cpu_halted
	call	println
.halt	cli
	hlt
	jmp	.halt
	mov	rsi, [rsp+reg.rsi]
	mov	rdi, [rsp+reg.rdi]
	mov	rax, [rsp+reg.rax]
	add	rsp, reg.size
	iretq

	global PageFaultHandler
PageFaultHandler:
	cli
	sub	rsp, reg.size
	savregs	rsp
	mov	rax, [rsp+reg.size+8]	; fault rip
	mov	[rsp+reg.rip], rax
	mov	ax, [rsp+reg.size+10h]	; fault cs
	mov	[rsp+reg.cs], ax
	mov	rax, [rsp+reg.size+18h]	; fault rflags
	mov	[rsp+reg.rflags], rax
	mov	rax, [rsp+reg.size+20h]	; fault rsp
	mov	[rsp+reg.rsp], rax
	mov	ax, [rsp+reg.size+28h]	; fault ss
	mov	[rsp+reg.ss], ax
	mov	rdi, [rsp+reg.size]	; fault error code
	mov	rsi, rsp		; pointer to regs struct
	call	PageFaultHandlerM
	mov	rdi, cpu_halted
	call	println
.halt	cli
	hlt
	jmp	.halt
	mov	rsi, [rsp+reg.rsi]
	mov	rdi, [rsp+reg.rdi]
	mov	rax, [rsp+reg.rax]
	add	rsp, reg.size
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
	mov	ax, 0x42
	mov	ds, ax
	ret

	global CausePageFault
CausePageFault:
	mov	[0x442], eax
	ret

	section .data

	global timer_count
timer_count:
	dd 0

cpu_halted:
	db "CPU halted", 0
