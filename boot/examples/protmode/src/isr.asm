	bits 32

	extern keyboard_handler_main

	global divbyzero_fault
divbyzero_fault:
	inc	dword [timer_count]
	iret

	global gp_fault
gp_fault:
	inc	dword [timer_count]
	iret

	global timer_isr
timer_isr:
	push	eax
	inc	dword [timer_count]
	mov	al, 0x20
	out	0x20, al	; issue EOI
	pop	eax
	iret

	global keyboard_handler
keyboard_handler:
	call	keyboard_handler_main
	iretd

	global load_idt
load_idt:
	mov eax, [esp + 4]
	lidt [eax]
	sti
	ret

	section .data

	global timer_count
timer_count:
	dd 0
