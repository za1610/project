	.file	"add2.c"
	.section	.rodata
.LC2:
	.string	"float %f %x\n"
.LC5:
	.string	"double %lf %lx\n"
.LC6:
	.string	"result is %lf\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movl	$0x4121f7cf, %eax
	movl	%eax, -12(%rbp)
	movl	$0x3dfbe76d, %eax
	movl	%eax, -8(%rbp)
	movss	-12(%rbp), %xmm0
	addss	-8(%rbp), %xmm0
	movss	%xmm0, -16(%rbp)
	leaq	-16(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movss	-16(%rbp), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC2, %eax
	movl	-4(%rbp), %edx
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	movabsq	$4621888360433242341, %rax
	movq	%rax, -56(%rbp)
	movabsq	$4593527504729830064, %rax
	movq	%rax, -48(%rbp)
	movsd	-56(%rbp), %xmm0
	addsd	-48(%rbp), %xmm0
	movsd	%xmm0, -72(%rbp)
	leaq	-72(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -40(%rbp)
	movsd	-72(%rbp), %xmm0
	movl	$.LC5, %eax
	movq	-40(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	movss	-16(%rbp), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	movsd	%xmm0, -64(%rbp)
	leaq	-64(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -32(%rbp)
	movsd	-64(%rbp), %xmm0
	movl	$.LC5, %eax
	movq	-32(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	movsd	-72(%rbp), %xmm1
	movss	-16(%rbp), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	movapd	%xmm1, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	movsd	%xmm0, -24(%rbp)
	movl	$.LC6, %eax
	movsd	-24(%rbp), %xmm0
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
