	section .text

	bits 32

	extern DumpException
	extern DisplayText
	extern KeyboardHandlerMain

GetEIP:
	mov eax, [esp]
	ret

	global GPFaultHandler
GPFaultHandler:
	push	gp_fault
	call	DumpException
	add	sp, 4
.halt	cli
	hlt
	jmp .halt
	iret

	global PageFaultHandler
PageFaultHandler:
	push	page_fault
	call	DumpException
	add	sp, 4
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
	mov	eax, [0]
	ret

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
