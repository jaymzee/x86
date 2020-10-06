	.global	_start

	.section .rodata
msg:
	.ascii	"hello world!\n"
	.set msglen, . - msg

	.text
_start:
	jmp	main
main:
	mov	$4,%eax		# use the write syscall
	mov	$1,%ebx		# write to stdout
	mov	$msg,%ecx	# use string "Hello World"
	mov	$msglen,%edx	# write 12 characters
	int	$0x80		# make syscall

	mov	$1,%eax		# use the _exit syscall
	mov	$0,%ebx		# error code 0
	int	$0x80		# make syscall
