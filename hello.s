# hello.s
.global _start

.text
_start:
	# write(1, message. 14)
	mov	$1,	%rax	# system call (sys_write)
	mov	$1,	%rdi	# arg #1 (stdout)
	mov	$msg,	%rsi	# arg #2 (string to display
	mov	$14,	%rdx	# arg #3 (length of string)
	syscall

	mov	$60,	%rax	# system call (exit)
	mov	$0,	%rdi	# exit code (0 = success)
	syscall

msg:
	.ascii	"Hello, world!\n"
