struc reg
.rax	resq	1	; 0
.rcx	resq	1	; 8
.rdx	resq	1	; 16
.rbx	resq	1	; 24
.rsp	resq	1	; 32
.rbp	resq	1	; 40
.rsi	resq	1	; 48
.rdi	resq	1	; 56
.r8 	resq	1	; 64
.r9 	resq	1	; 72
.r10	resq	1	; 80
.r11	resq	1	; 88
.r12	resq	1	; 96
.r13	resq	1	; 104
.r14	resq	1	; 112
.r15	resq	1	; 120
.rflags	resq	1	; 128
.rip	resq	1	; 136
.cr0	resq	1	; 144
.cr2	resq	1	; 152
.cr3	resq	1	; 160
.cr4	resq	1	; 168
.cr8	resq	1	; 176
.es 	resw	1	; 184
.cs 	resw	1	; 186
.ss 	resw	1	; 188
.ds 	resw	1	; 190
.fs 	resw	1	; 192
.gs 	resw	1	; 194
.size	equ	($ + 15) & 0xFFF0
endstruc

; eax (rax) is clobbered but it is saved first
%macro	savregs	1
	mov	[%1+reg.rax], rax
	mov	[%1+reg.rcx], rcx
	mov	[%1+reg.rdx], rdx
	mov	[%1+reg.rbx], rbx
	mov	[%1+reg.rsp], rsp
	mov	[%1+reg.rbp], rbp
	mov	[%1+reg.rsi], rsi
	mov	[%1+reg.rdi], rdi
	mov	[%1+reg.r8], r8
	mov	[%1+reg.r9], r9
	mov	[%1+reg.r10], r10
	mov	[%1+reg.r11], r11
	mov	[%1+reg.r12], r12
	mov	[%1+reg.r13], r13
	mov	[%1+reg.r14], r14
	mov	[%1+reg.r15], r15
	mov	eax, es
	mov	[%1+reg.es], ax
	mov	eax, cs
	mov	[%1+reg.cs], ax
	mov	eax, ss
	mov	[%1+reg.ss], ax
	mov	eax, ds
	mov	[%1+reg.ds], ax
	mov	eax, fs
	mov	[%1+reg.fs], ax
	mov	eax, gs
	mov	[%1+reg.gs], ax
	mov	rax, cr0
	mov	[%1+reg.cr0], rax
	mov	rax, cr2
	mov	[%1+reg.cr2], rax
	mov	rax, cr3
	mov	[%1+reg.cr3], rax
	mov	rax, cr4
	mov	[%1+reg.cr4], rax
	mov	rax, cr8
	mov	[%1+reg.cr8], rax
%endmacro

