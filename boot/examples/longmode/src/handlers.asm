	section .text

	bits 64

	extern DumpException
	extern KeyboardHandlerMain

	global KeyboardHandler
KeyboardHandler:
	push	rdi
	push	rsi
	push	rax
	push	rcx
	push	rdx
	mov	al, 0x20
	out	0x20, al	; issue EOI
	call	KeyboardHandlerMain
	pop	rdx
	pop	rcx
	pop	rax
	pop	rsi
	pop	rdi
	iretq

	global TimerHandler
TimerHandler:
	push	rax
	mov	al, 0x20
	out	0x20, al	; issue EOI
	inc	dword [timer_count]
	pop	rax
	iretq

	section .data

	global timer_count
timer_count:
	dd 0

div_by_zero:
	db "division by zero, cpu halted",0
gp_fault:
	db "GP Fault",0
page_fault:
	db "Page Fault",0
