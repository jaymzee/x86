	bits 32

	extern DisplayText
	extern keyboard_handler_main

get_eip:
	mov eax, [esp]
	ret

	global gpfault_handler
gpfault_handler:
	push gpfault
	call DisplayText
	pop ecx
.halt	hlt
	jmp .halt
	iret

	global divbyzero_handler
divbyzero_handler:
	push divbyzero
	call DisplayText
	pop ecx
.halt	hlt
	jmp .halt
	iret

	global keyboard_handler
keyboard_handler:
	call	keyboard_handler_main
	iret

	global timer_handler
timer_handler:
	push	eax
	inc	dword [timer_count]
	mov	al, 0x20
	out	0x20, al	; issue EOI
	pop	eax
	iret

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

divbyzero:
	db "division by zero, cpu halted",0
gpfault:
	db "general protection fault, cpu halted",0
