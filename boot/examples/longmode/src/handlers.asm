%include "x86_64.asm"

	section .text

	bits 64

	extern DumpError
	extern DumpException
	extern KeyboardHandlerMain

	global GPFaultHandler
GPFaultHandler:
	sub	rsp, 200
	mov	[rsp+reg.rax], rax
	mov	[rsp+reg.rcx], rcx
	mov	[rsp+reg.rdx], rdx
	mov	[rsp+reg.rbx], rbx
	mov	[rsp+reg.rsp], rsp
	mov	[rsp+reg.rbp], rbp
	mov	[rsp+reg.rsi], rsi
	mov	[rsp+reg.rdi], rdi
	mov	[rsp+reg.r8], r8
	mov	[rsp+reg.r9], r9
	mov	[rsp+reg.r10], r10
	mov	[rsp+reg.r11], r11
	mov	[rsp+reg.r12], r12
	mov	[rsp+reg.r13], r13
	mov	[rsp+reg.r14], r14
	mov	[rsp+reg.r15], r15
	mov	rax, [rsp+224]		; rflags
	mov	[rsp+reg.rflags], rax
	mov	rax, [rsp+208]		; rip
	mov	[rsp+reg.rip], rax
	mov	eax, es
	mov	[rsp+reg.es], ax
	mov	eax, [rsp+216]		; cs
	mov	[rsp+reg.cs], ax
	mov	eax, ss
	mov	[rsp+reg.ss], ax
	mov	eax, ds
	mov	[rsp+reg.ds], ax
	mov	eax, fs
	mov	[rsp+reg.fs], ax
	mov	eax, gs
	mov	[rsp+reg.gs], ax
	mov	rdi, gp_fault
	mov	rsi, [rsp+200]		; error code
	mov	rdx, rsp		; regs struct
	call	DumpError
.halt	cli
	hlt
	jmp	.halt
	add	rsp, 200
	iretq

	global KeyboardHandler
KeyboardHandler:
	push	rdi
	push	rsi
	push	rax
	push	rcx
	push	rdx
	mov	al, 0x20
	out	0x20, al		; issue EOI
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
	out	0x20, al		; issue EOI
	inc	dword [timer_count]
	pop	rax
	iretq

	global CrashMe
CrashMe:
	mov	ax, 0x42
	mov	ds, ax
	ret

	section .data

	global timer_count
timer_count:
	dd 0

div_by_zero:
	db "division by zero, cpu halted", 0
gp_fault:
	db "GP Fault", 0
page_fault:
	db "Page Fault", 0
