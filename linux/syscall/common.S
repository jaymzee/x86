#ifndef COMMON_S
#define COMMON_S

#include <asm/unistd.h>

.macro	write	fd, msg, msglen
	mov	$__NR_write, %rax	# use the write syscall
	mov	\fd, %rdi		# file descriptor
	lea	\msg(%rip), %rsi		# string to write
	mov	\msglen, %rdx		# number of characters
	syscall				# make syscall
.endm

.macro	exit	exitcode
	mov	$__NR_exit, %rax	# use the _exit syscall
	mov	\exitcode, %rdi		# exit code
	syscall
.endm

#endif /* COMMON_S */
