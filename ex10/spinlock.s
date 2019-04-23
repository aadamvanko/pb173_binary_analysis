	.text
	.globl	spin_lock
	.p2align	4, 0x90
	.type	spin_lock,@function
spin_lock:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	%rdi, %rbx
	
try_lock:
	movq	$1, %rcx
	movq	$0, %rax
	lock; cmpxchg	%rcx, (%rbx) 
	jnz try_lock
	
	popq	%rbp
	retq

	.text
	.globl	spin_unlock
	.p2align	4, 0x90
	.type	spin_unlock,@function

spin_unlock:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	%rdi, %rbx

	movq $0, (%rbx)	

	popq	%rbp
	retq
