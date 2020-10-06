#include "common.S"

	.global	_start
	.data
msg:
	.ascii	"hello world!\n"
	msglen = . - msg
	.text
_start:
	write	$1, $msg, $msglen
	exit	$0
