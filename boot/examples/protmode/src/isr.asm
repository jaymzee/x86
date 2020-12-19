	bits 32

	extern keyboard_handler_main

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
