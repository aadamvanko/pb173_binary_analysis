main:                                   # @main
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$0x777, -1(%rip)
	movl	$0, -4(%rbp)
	callq	funA
	nop
	nop
	jmp	0x4000a7
	movl	%eax, -8(%rbp)
	jmp	0x2
	movl	%ecx, %eax
	popq	%rbp
	retq
funA:                                   # @funA
	pushq	%rbp
	call main	
	movq	%rsp, %rbp
	movl	$123, +2(%rip)
	jmp	nop_label
	nop
	callq funA
nop_label:
	nop
	movl	-4(%rbp), %eax
jb_label:
	jb	je_label
	nop
	nop
je_label:
	je	jb_label
	jne	pop_label
	nop
	nop
	nop
	nop
	nop
pop_label:
	popq	%rbp
	callq	some_function
	retq
some_function:
	pushq	%rbp
	movq	%rsp, %rbp
	pop	%rbp
	retq
