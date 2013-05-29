	.file	"m2.c"
	.comm	A,680000,32
	.text
	.globl	rand_FloatRange
	.type	rand_FloatRange, @function
rand_FloatRange:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movss	%xmm0, -4(%rbp)
	movss	%xmm1, -8(%rbp)
	movss	-8(%rbp), %xmm0
	movaps	%xmm0, %xmm1
	subss	-4(%rbp), %xmm1
	movss	%xmm1, -12(%rbp)
	call	rand
	cvtsi2ss	%eax, %xmm0
	movss	.LC0(%rip), %xmm1
	divss	%xmm1, %xmm0
	mulss	-12(%rbp), %xmm0
	addss	-4(%rbp), %xmm0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	rand_FloatRange, .-rand_FloatRange
	.globl	fillRand
	.type	fillRand, @function
fillRand:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$56, %rsp
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movl	%edx, -48(%rbp)
	movss	%xmm0, -52(%rbp)
	movss	%xmm1, -56(%rbp)
	movl	$123, %edi
	.cfi_offset 3, -24
	call	srand
	movl	-44(%rbp), %eax
	movl	%eax, -20(%rbp)
	jmp	.L3
.L4:
	movl	-20(%rbp), %eax
	cltq
	salq	$2, %rax
	movq	%rax, %rbx
	addq	-40(%rbp), %rbx
	movss	-56(%rbp), %xmm1
	movss	-52(%rbp), %xmm0
	call	rand_FloatRange
	movss	%xmm0, (%rbx)
	addl	$1, -20(%rbp)
.L3:
	movl	-20(%rbp), %eax
	cmpl	-48(%rbp), %eax
	jl	.L4
	addq	$56, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	fillRand, .-fillRand
	.globl	compare
	.type	compare, @function
compare:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -8(%rbp)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movss	-8(%rbp), %xmm0
	ucomiss	-4(%rbp), %xmm0
	seta	%al
	movzbl	%al, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	compare, .-compare
	.globl	fillRandSorted
	.type	fillRandSorted, @function
fillRandSorted:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movss	%xmm0, -20(%rbp)
	movss	%xmm1, -24(%rbp)
	movss	-24(%rbp), %xmm1
	movss	-20(%rbp), %xmm0
	movl	-16(%rbp), %edx
	movl	-12(%rbp), %ecx
	movq	-8(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	fillRand
	movq	-8(%rbp), %rax
	movl	$compare, %ecx
	movl	$4, %edx
	movl	$170000, %esi
	movq	%rax, %rdi
	call	qsort
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	fillRandSorted, .-fillRandSorted
	.globl	fillOnes
	.type	fillOnes, @function
fillOnes:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	%edx, -32(%rbp)
	movss	%xmm0, -36(%rbp)
	movss	%xmm1, -40(%rbp)
	movl	-28(%rbp), %eax
	movl	%eax, -4(%rbp)
	jmp	.L8
.L9:
	movl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-24(%rbp), %rax
	movl	$0x3f800000, %edx
	movl	%edx, (%rax)
	addl	$1, -4(%rbp)
.L8:
	movl	-4(%rbp), %eax
	cmpl	-32(%rbp), %eax
	jl	.L9
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	fillOnes, .-fillOnes
	.globl	fsumUp
	.type	fsumUp, @function
fsumUp:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	$0x00000000, %eax
	movl	%eax, -8(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L11
.L12:
	movl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-24(%rbp), %rax
	movss	(%rax), %xmm0
	movss	-8(%rbp), %xmm1
	addss	%xmm1, %xmm0
	movss	%xmm0, -8(%rbp)
	addl	$1, -4(%rbp)
.L11:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L12
	movl	-8(%rbp), %eax
	movl	%eax, -32(%rbp)
	movss	-32(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	fsumUp, .-fsumUp
	.globl	fsumUp2
	.type	fsumUp2, @function
fsumUp2:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	$0x00000000, %eax
	movl	%eax, -8(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L14
.L15:
	movl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-24(%rbp), %rax
	movss	(%rax), %xmm0
	movss	-8(%rbp), %xmm1
	addss	%xmm1, %xmm0
	movss	%xmm0, -8(%rbp)
	addl	$1, -4(%rbp)
.L14:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L15
	movl	-8(%rbp), %eax
	movl	%eax, -32(%rbp)
	movss	-32(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	fsumUp2, .-fsumUp2
	.globl	fsumDown
	.type	fsumDown, @function
fsumDown:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	$0x00000000, %eax
	movl	%eax, -8(%rbp)
	movl	-28(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -4(%rbp)
	jmp	.L17
.L18:
	movl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-24(%rbp), %rax
	movss	(%rax), %xmm0
	movss	-8(%rbp), %xmm1
	addss	%xmm1, %xmm0
	movss	%xmm0, -8(%rbp)
	subl	$1, -4(%rbp)
.L17:
	cmpl	$0, -4(%rbp)
	jns	.L18
	movl	-8(%rbp), %eax
	movl	%eax, -32(%rbp)
	movss	-32(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	fsumDown, .-fsumDown
	.globl	dsumUp
	.type	dsumUp, @function
dsumUp:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	$0, %eax
	movq	%rax, -16(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L20
.L21:
	movl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-24(%rbp), %rax
	movss	(%rax), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	movsd	-16(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
	addl	$1, -4(%rbp)
.L20:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L21
	movq	-16(%rbp), %rax
	movq	%rax, -40(%rbp)
	movsd	-40(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	dsumUp, .-dsumUp
	.globl	dsumDown
	.type	dsumDown, @function
dsumDown:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	$0, %eax
	movq	%rax, -16(%rbp)
	movl	-28(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -4(%rbp)
	jmp	.L23
.L24:
	movl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-24(%rbp), %rax
	movss	(%rax), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	movsd	-16(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
	subl	$1, -4(%rbp)
.L23:
	cmpl	$0, -4(%rbp)
	jns	.L24
	movq	-16(%rbp), %rax
	movq	%rax, -40(%rbp)
	movsd	-40(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	dsumDown, .-dsumDown
	.globl	kahan_summationUp
	.type	kahan_summationUp, @function
kahan_summationUp:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movl	$0x00000000, %eax
	movl	%eax, -20(%rbp)
	movl	$0x00000000, %eax
	movl	%eax, -16(%rbp)
	movl	$0, -12(%rbp)
	jmp	.L26
.L27:
	movl	-12(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-40(%rbp), %rax
	movss	(%rax), %xmm0
	subss	-16(%rbp), %xmm0
	movss	%xmm0, -8(%rbp)
	movss	-20(%rbp), %xmm0
	addss	-8(%rbp), %xmm0
	movss	%xmm0, -4(%rbp)
	movss	-4(%rbp), %xmm0
	subss	-20(%rbp), %xmm0
	subss	-8(%rbp), %xmm0
	movss	%xmm0, -16(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, -20(%rbp)
	addl	$1, -12(%rbp)
.L26:
	movl	-12(%rbp), %eax
	cmpl	-44(%rbp), %eax
	jl	.L27
	movl	-20(%rbp), %eax
	movl	%eax, -48(%rbp)
	movss	-48(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	kahan_summationUp, .-kahan_summationUp
	.globl	kahan_summationDown
	.type	kahan_summationDown, @function
kahan_summationDown:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movl	$0x00000000, %eax
	movl	%eax, -20(%rbp)
	movl	$0x00000000, %eax
	movl	%eax, -16(%rbp)
	movl	-44(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -12(%rbp)
	jmp	.L29
.L30:
	movl	-12(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-40(%rbp), %rax
	movss	(%rax), %xmm0
	subss	-16(%rbp), %xmm0
	movss	%xmm0, -8(%rbp)
	movss	-20(%rbp), %xmm0
	addss	-8(%rbp), %xmm0
	movss	%xmm0, -4(%rbp)
	movss	-4(%rbp), %xmm0
	subss	-20(%rbp), %xmm0
	subss	-8(%rbp), %xmm0
	movss	%xmm0, -16(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, -20(%rbp)
	subl	$1, -12(%rbp)
.L29:
	cmpl	$0, -12(%rbp)
	jns	.L30
	movl	-20(%rbp), %eax
	movl	%eax, -48(%rbp)
	movss	-48(%rbp), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	kahan_summationDown, .-kahan_summationDown
	.globl	fsumRecursive
	.type	fsumRecursive, @function
fsumRecursive:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	cmpl	$0, -28(%rbp)
	jne	.L32
	xorps	%xmm0, %xmm0
	jmp	.L33
.L32:
	cmpl	$1, -28(%rbp)
	jne	.L34
	movq	-24(%rbp), %rax
	movss	(%rax), %xmm0
	jmp	.L33
.L34:
	movl	-28(%rbp), %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %edx
	movq	-24(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	fsumRecursive
	movss	%xmm0, -8(%rbp)
	movl	-28(%rbp), %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, %ecx
	movl	-28(%rbp), %eax
	movl	%eax, %edx
	sarl	$31, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	andl	$1, %eax
	subl	%edx, %eax
	leal	(%rcx,%rax), %edx
	movl	-28(%rbp), %eax
	movl	%eax, %ecx
	shrl	$31, %ecx
	addl	%ecx, %eax
	sarl	%eax
	cltq
	salq	$2, %rax
	addq	-24(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	fsumRecursive
	movss	%xmm0, -4(%rbp)
	movss	-8(%rbp), %xmm0
	addss	-4(%rbp), %xmm0
.L33:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	fsumRecursive, .-fsumRecursive
	.globl	DoTheSummation
	.type	DoTheSummation, @function
DoTheSummation:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movl	-44(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	fsumUp
	movss	%xmm0, -12(%rbp)
	movl	-44(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	fsumUp2
	movss	%xmm0, -8(%rbp)
	movl	-44(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	fsumDown
	movss	%xmm0, -4(%rbp)
	movl	-44(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	dsumUp
	movsd	%xmm0, -32(%rbp)
	movl	-44(%rbp), %edx
	movq	-40(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	dsumDown
	movsd	%xmm0, -24(%rbp)
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	DoTheSummation, .-DoTheSummation
	.globl	printItForDebug
	.type	printItForDebug, @function
printItForDebug:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	printItForDebug, .-printItForDebug
	.section	.rodata
.LC4:
	.string	"Ones:      "
.LC6:
	.string	"Rand:      "
.LC9:
	.string	"RandSorted:"
	.text
	.globl	main
	.type	main, @function
main:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$.LC4, %edi
	call	puts
	movss	.LC1(%rip), %xmm1
	movss	.LC5(%rip), %xmm0
	movl	$170000, %edx
	movl	$0, %esi
	movl	$A, %edi
	call	fillOnes
	movl	$170000, %esi
	movl	$A, %edi
	call	DoTheSummation
	movl	$170000, %esi
	movl	$A, %edi
	call	printItForDebug
	movl	$.LC6, %edi
	call	puts
	movss	.LC7(%rip), %xmm1
	movss	.LC8(%rip), %xmm0
	movl	$170000, %edx
	movl	$0, %esi
	movl	$A, %edi
	call	fillRand
	movl	$170000, %esi
	movl	$A, %edi
	call	DoTheSummation
	movl	$170000, %esi
	movl	$A, %edi
	call	printItForDebug
	movl	$.LC9, %edi
	call	puts
	movss	.LC7(%rip), %xmm1
	movss	.LC8(%rip), %xmm0
	movl	$170000, %edx
	movl	$0, %esi
	movl	$A, %edi
	call	fillRandSorted
	movl	$170000, %esi
	movl	$A, %edi
	call	DoTheSummation
	movl	$170000, %esi
	movl	$A, %edi
	call	printItForDebug
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1325400064
	.align 4
.LC1:
	.long	1065353216
	.align 4
.LC5:
	.long	3212836864
	.align 4
.LC7:
	.long	1176256512
	.align 4
.LC8:
	.long	3323740160
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
