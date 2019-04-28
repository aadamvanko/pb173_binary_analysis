	.text
	.file	"test_binary.c"
	.globl	funA                    # -- Begin function funA
	.p2align	4, 0x90
	.type	funA,@function
funA:                                   # @funA
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$123, -4(%rbp)
	movl	-4(%rbp), %eax
	addl	$12, %eax
	popq	%rbp
	retq
.Lfunc_end0:
	.size	funA, .Lfunc_end0-funA
	.cfi_endproc
                                        # -- End function
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movl	$0x13, -20(%rip)
	movl	$0, -4(%rbp)
	movl	$0x5, 4(%rip)
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)"
	.section	".note.GNU-stack","",@progbits
