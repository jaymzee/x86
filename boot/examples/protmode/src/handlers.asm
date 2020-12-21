	section .text

	bits 32

	extern DisplayText
	extern KeyboardHandlerMain

GetEIP:
	mov eax, [esp]
	ret

	global GPFaultHandler
GPFaultHandler:
	push gp_fault
	call DisplayText
	pop ecx
	mov eax,[esp]
.halt	cli
	hlt
	jmp .halt
	iret

	global DivByZeroHandler
DivByZeroHandler:
	push div_by_zero
	call DisplayText
	pop ecx
.halt	cli
	hlt
	jmp .halt
	iret

	global KeyboardHandler
KeyboardHandler:
	push eax
	push ecx
	push edx
	mov	al, 0x20
	out	0x20, al	; issue EOI
	call	KeyboardHandlerMain
	pop	edx
	pop	ecx
	pop	eax
	iret

	global TimerHandler
TimerHandler:
	push	eax
	inc	dword [timer_count]
	mov	al, 0x20
	out	0x20, al	; issue EOI
	pop	eax
	iret

	global CrashMe
CrashMe:
	mov	eax, 42
	mov	ds, eax
	mov	eax, [0x1000]
	ret

	section .data

	global timer_count
timer_count:
	dd 0

div_by_zero:
	db "division by zero, cpu halted",0
gp_fault:
	db "general protection fault, cpu halted",0
