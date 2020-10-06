; Hello World Win64
; Display Hello World in a MessageBox

includelib kernel32.lib
includelib user32.lib

extrn MessageBoxA: PROC
extrn ExitProcess: PROC

.data
caption db 'Win64', 0
msg db 'Hello World', 0

.code
main PROC FRAME
	push	rbp
.pushreg rbp
	mov	rbp,rsp
.setframe rbp,0
	sub	rsp,20h
.allocstack 20h
.endprolog
	mov	rcx,0		; hWnd = HWND_DESKTOP
	lea	rdx,msg		; LPCSTR lpText
	lea	r8,caption	; LPCSTR lpCaption
	mov	r9d,0		; uType = MB_OK
	call	MessageBoxA
	mov	ecx,eax		; uExitCode = MessageBox(...)
	call	ExitProcess
	leave
main ENDP

END
