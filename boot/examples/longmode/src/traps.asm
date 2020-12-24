%include "cpu64.asm"

	extern CPUException
	extern WriteText
	extern puts

%macro	cpuexcepterr 1
	cli
	push	rbp
	mov	rbp, rsp
	sub	rsp, reg.size+8
	savregs	rsp
	savisf	rsp, rbp+10h		; fault rip, rflags, rsp, etc.
	mov	rdi, rsp		; pointer to reg struct
	mov	rsi, %1
	mov	rdx, [rbp+8h]		; fault error code
	call	CPUException			; dump registers
	jmp	halt_system
%endmacro

%macro	cpuexcept 1
	cli
	push	rbp
	mov	rbp, rsp
	sub	rsp, reg.size+8
	savregs	rsp
	savisf	rsp, rbp+8		; fault rip, rflags, rsp, etc.
	mov	rdi, rsp		; pointer to regs struct
	mov	rsi, %1
	mov	rdx, 0			; no error code so set to 0
	call	CPUException
	jmp	halt_system
%endmacro

	bits 64
	section .text

; prototype for a cpu exception with an error code
%if 0
ExceptionWithErrorHandler:
	cli
	push	rbp
	mov	rbp, rsp
	call	some_c_function
	leave				; cleanup frame
	add	rsp, 8			; pop error code so rsp -> ret addr
	iretq
%endif

; cpu exceptions

	; Divide-by-zero Error: Fault
	global DivbyzeroHandler
DivbyzeroHandler:
	cpuexcept 0x0

	; Non-maskable Interrupt: Interrupt
	global NMIHandler
NMIHandler:
	cpuexcept 0x2

	; Invalid Opcode: Fault
	global InvalidOpcodeHandler
InvalidOpcodeHandler:
	cpuexcept 0x6

	; Double Fault: Abort with error code
	global DoubleFaultHandler
DoubleFaultHandler:
	cpuexcepterr 0x8

	; Invalid TSS: Fault with error code
	global InvalidTSSHandler
InvalidTSSHandler:
	cpuexcepterr 0xa

	; SegNotPresent: Fault with error code
	global SegNotPresentHandler
SegNotPresentHandler:
	cpuexcepterr 0xb

	; StackSegFault: Fault with error code
	global StackSegFaultHandler
StackSegFaultHandler:
	cpuexcepterr 0xc

	; General Protection Fault: Fault with error code
	global GPFaultHandler
GPFaultHandler:
	cpuexcepterr 0xd

	; Page Fault: Fault with error code
	global PageFaultHandler
PageFaultHandler:
	cpuexcepterr 0xe

	; x87 Floating-Point Exception: Fault
	global x87FPExceptHandler
x87FPExceptHandler:
	cpuexcept 0x10

	; Alignment Check: Fault with error code
	global AlignCheckHandler
AlignCheckHandler:
	cpuexcepterr 0x11

	; Machine Check: Abort
	global MachineCheckHandler
MachineCheckHandler:
	cpuexcept 0x12

	; SIMD Floating-Point Exception: Fault
	global SIMDFPExceptHandler
SIMDFPExceptHandler:
	cpuexcept 0x13

	; Virtualization Exception: Fault
	global VirtExceptHandler
VirtExceptHandler:
	cpuexcept 0x14

	; Security Exception: - with error
	global SecurityExceptHandler
SecurityExceptHandler:
	cpuexcepterr 0x1e

halt_system:
	mov	rdi, system_halted
	call	WriteText
	mov	rdi, system_halted
	call	puts
.halt	cli
	hlt
	jmp	.halt

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
system_halted:
	db `system halted.\n`, 0
