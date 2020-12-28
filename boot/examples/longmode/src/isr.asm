	extern KeyboardHandlerM

	bits 64
	section .text

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
	call	KeyboardHandlerM
	mov	al, 0x20
	out	0x20, al	; issue EOI
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

	section .data

	global timer_count
timer_count:
	dd 0
