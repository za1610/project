	.file	"m2.c"
	.text
.Ltext0:
	.comm	A,64000000,32
	.globl	rand_FloatRange
	.type	rand_FloatRange, @function
rand_FloatRange:
.LFB0:
	.file 1 "m2.c"
	.loc 1 18 0
	.cfi_startproc
	pushq	%rbp
.LCFI0:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI1:
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movss	%xmm0, -4(%rbp)
	movss	%xmm1, -8(%rbp)
	.loc 1 19 0
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
	.loc 1 20 0
	leave
.LCFI2:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	rand_FloatRange, .-rand_FloatRange
	.globl	fillRand
	.type	fillRand, @function
fillRand:
.LFB1:
	.loc 1 23 0
	.cfi_startproc
	pushq	%rbp
.LCFI3:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI4:
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$56, %rsp
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movl	%edx, -48(%rbp)
	movss	%xmm0, -52(%rbp)
	movss	%xmm1, -56(%rbp)
	.loc 1 24 0
	movl	$123, %edi
	.cfi_offset 3, -24
	call	srand
.LBB2:
	.loc 1 25 0
	movl	-44(%rbp), %eax
	movl	%eax, -20(%rbp)
	jmp	.L3
.L4:
	.loc 1 27 0 discriminator 2
	movl	-20(%rbp), %eax
	cltq
	salq	$2, %rax
	movq	%rax, %rbx
	addq	-40(%rbp), %rbx
	movss	-56(%rbp), %xmm1
	movss	-52(%rbp), %xmm0
	call	rand_FloatRange
	movss	%xmm0, (%rbx)
	.loc 1 25 0 discriminator 2
	addl	$1, -20(%rbp)
.L3:
	.loc 1 25 0 is_stmt 0 discriminator 1
	movl	-20(%rbp), %eax
	cmpl	-48(%rbp), %eax
	jl	.L4
.LBE2:
	.loc 1 29 0 is_stmt 1
	addq	$56, %rsp
	popq	%rbx
	popq	%rbp
.LCFI5:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	fillRand, .-fillRand
	.globl	compare
	.type	compare, @function
compare:
.LFB2:
	.loc 1 30 0
	.cfi_startproc
	pushq	%rbp
.LCFI6:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI7:
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	.loc 1 30 0
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
.LCFI8:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	compare, .-compare
	.globl	fillRandSorted
	.type	fillRandSorted, @function
fillRandSorted:
.LFB3:
	.loc 1 32 0
	.cfi_startproc
	pushq	%rbp
.LCFI9:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI10:
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movss	%xmm0, -20(%rbp)
	movss	%xmm1, -24(%rbp)
	.loc 1 33 0
	movss	-24(%rbp), %xmm1
	movss	-20(%rbp), %xmm0
	movl	-16(%rbp), %edx
	movl	-12(%rbp), %ecx
	movq	-8(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	fillRand
	.loc 1 35 0
	movq	-8(%rbp), %rax
	movl	$compare, %ecx
	movl	$4, %edx
	movl	$16000000, %esi
	movq	%rax, %rdi
	call	qsort
	.loc 1 36 0
	leave
.LCFI11:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	fillRandSorted, .-fillRandSorted
	.globl	fillOnes
	.type	fillOnes, @function
fillOnes:
.LFB4:
	.loc 1 39 0
	.cfi_startproc
	pushq	%rbp
.LCFI12:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI13:
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	%edx, -32(%rbp)
	movss	%xmm0, -36(%rbp)
	movss	%xmm1, -40(%rbp)
.LBB3:
	.loc 1 40 0
	movl	-28(%rbp), %eax
	movl	%eax, -4(%rbp)
	jmp	.L8
.L9:
	.loc 1 42 0 discriminator 2
	movl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-24(%rbp), %rax
	movl	$0x3f800000, %edx
	movl	%edx, (%rax)
	.loc 1 40 0 discriminator 2
	addl	$1, -4(%rbp)
.L8:
	.loc 1 40 0 is_stmt 0 discriminator 1
	movl	-4(%rbp), %eax
	cmpl	-32(%rbp), %eax
	jl	.L9
.LBE3:
	.loc 1 44 0 is_stmt 1
	popq	%rbp
.LCFI14:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	fillOnes, .-fillOnes
	.globl	fsumUp
	.type	fsumUp, @function
fsumUp:
.LFB5:
	.loc 1 47 0
	.cfi_startproc
	pushq	%rbp
.LCFI15:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI16:
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	.loc 1 50 0
	movl	$0x00000000, %eax
	movl	%eax, -8(%rbp)
.LBB4:
	.loc 1 53 0
	movl	$0, -4(%rbp)
	jmp	.L11
.L12:
	.loc 1 55 0 discriminator 2
	movl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-24(%rbp), %rax
	movss	(%rax), %xmm0
	movss	-8(%rbp), %xmm1
	addss	%xmm1, %xmm0
	movss	%xmm0, -8(%rbp)
	.loc 1 53 0 discriminator 2
	addl	$1, -4(%rbp)
.L11:
	.loc 1 53 0 is_stmt 0 discriminator 1
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L12
.LBE4:
	.loc 1 59 0 is_stmt 1
	movl	-8(%rbp), %eax
	movl	%eax, -32(%rbp)
	movss	-32(%rbp), %xmm0
	.loc 1 60 0
	popq	%rbp
.LCFI17:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	fsumUp, .-fsumUp
	.globl	fsumUp2
	.type	fsumUp2, @function
fsumUp2:
.LFB6:
	.loc 1 64 0
	.cfi_startproc
	pushq	%rbp
.LCFI18:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI19:
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	.loc 1 65 0
	movl	$0x00000000, %eax
	movl	%eax, -8(%rbp)
.LBB5:
	.loc 1 66 0
	movl	$0, -4(%rbp)
	jmp	.L14
.L15:
	.loc 1 67 0 discriminator 2
	movl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-24(%rbp), %rax
	movss	(%rax), %xmm0
	movss	-8(%rbp), %xmm1
	addss	%xmm1, %xmm0
	movss	%xmm0, -8(%rbp)
	.loc 1 66 0 discriminator 2
	addl	$1, -4(%rbp)
.L14:
	.loc 1 66 0 is_stmt 0 discriminator 1
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L15
.LBE5:
	.loc 1 69 0 is_stmt 1
	movl	-8(%rbp), %eax
	movl	%eax, -32(%rbp)
	movss	-32(%rbp), %xmm0
	.loc 1 70 0
	popq	%rbp
.LCFI20:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	fsumUp2, .-fsumUp2
	.globl	fsumDown
	.type	fsumDown, @function
fsumDown:
.LFB7:
	.loc 1 75 0
	.cfi_startproc
	pushq	%rbp
.LCFI21:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI22:
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	.loc 1 76 0
	movl	$0x00000000, %eax
	movl	%eax, -8(%rbp)
.LBB6:
	.loc 1 79 0
	movl	-28(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -4(%rbp)
	jmp	.L17
.L18:
	.loc 1 80 0 discriminator 2
	movl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-24(%rbp), %rax
	movss	(%rax), %xmm0
	movss	-8(%rbp), %xmm1
	addss	%xmm1, %xmm0
	movss	%xmm0, -8(%rbp)
	.loc 1 79 0 discriminator 2
	subl	$1, -4(%rbp)
.L17:
	.loc 1 79 0 is_stmt 0 discriminator 1
	cmpl	$0, -4(%rbp)
	jns	.L18
.LBE6:
	.loc 1 82 0 is_stmt 1
	movl	-8(%rbp), %eax
	movl	%eax, -32(%rbp)
	movss	-32(%rbp), %xmm0
	.loc 1 83 0
	popq	%rbp
.LCFI23:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	fsumDown, .-fsumDown
	.globl	dsumUp
	.type	dsumUp, @function
dsumUp:
.LFB8:
	.loc 1 86 0
	.cfi_startproc
	pushq	%rbp
.LCFI24:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI25:
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	.loc 1 87 0
	movl	$0, %eax
	movq	%rax, -16(%rbp)
.LBB7:
	.loc 1 88 0
	movl	$0, -4(%rbp)
	jmp	.L20
.L21:
	.loc 1 89 0 discriminator 2
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
	.loc 1 88 0 discriminator 2
	addl	$1, -4(%rbp)
.L20:
	.loc 1 88 0 is_stmt 0 discriminator 1
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L21
.LBE7:
	.loc 1 91 0 is_stmt 1
	movq	-16(%rbp), %rax
	movq	%rax, -40(%rbp)
	movsd	-40(%rbp), %xmm0
	.loc 1 92 0
	popq	%rbp
.LCFI26:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	dsumUp, .-dsumUp
	.globl	dsumDown
	.type	dsumDown, @function
dsumDown:
.LFB9:
	.loc 1 95 0
	.cfi_startproc
	pushq	%rbp
.LCFI27:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI28:
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	.loc 1 96 0
	movl	$0, %eax
	movq	%rax, -16(%rbp)
.LBB8:
	.loc 1 97 0
	movl	-28(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -4(%rbp)
	jmp	.L23
.L24:
	.loc 1 98 0 discriminator 2
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
	.loc 1 97 0 discriminator 2
	subl	$1, -4(%rbp)
.L23:
	.loc 1 97 0 is_stmt 0 discriminator 1
	cmpl	$0, -4(%rbp)
	jns	.L24
.LBE8:
	.loc 1 100 0 is_stmt 1
	movq	-16(%rbp), %rax
	movq	%rax, -40(%rbp)
	movsd	-40(%rbp), %xmm0
	.loc 1 101 0
	popq	%rbp
.LCFI29:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	dsumDown, .-dsumDown
	.globl	kahan_summationUp
	.type	kahan_summationUp, @function
kahan_summationUp:
.LFB10:
	.loc 1 103 0
	.cfi_startproc
	pushq	%rbp
.LCFI30:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI31:
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	.loc 1 104 0
	movl	$0x00000000, %eax
	movl	%eax, -20(%rbp)
	.loc 1 106 0
	movl	$0x00000000, %eax
	movl	%eax, -16(%rbp)
.LBB9:
	.loc 1 107 0
	movl	$0, -12(%rbp)
	jmp	.L26
.L27:
.LBB10:
	.loc 1 108 0 discriminator 2
	movl	-12(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-40(%rbp), %rax
	movss	(%rax), %xmm0
	subss	-16(%rbp), %xmm0
	movss	%xmm0, -8(%rbp)
	.loc 1 109 0 discriminator 2
	movss	-20(%rbp), %xmm0
	addss	-8(%rbp), %xmm0
	movss	%xmm0, -4(%rbp)
	.loc 1 110 0 discriminator 2
	movss	-4(%rbp), %xmm0
	subss	-20(%rbp), %xmm0
	subss	-8(%rbp), %xmm0
	movss	%xmm0, -16(%rbp)
	.loc 1 111 0 discriminator 2
	movl	-4(%rbp), %eax
	movl	%eax, -20(%rbp)
.LBE10:
	.loc 1 107 0 discriminator 2
	addl	$1, -12(%rbp)
.L26:
	.loc 1 107 0 is_stmt 0 discriminator 1
	movl	-12(%rbp), %eax
	cmpl	-44(%rbp), %eax
	jl	.L27
.LBE9:
	.loc 1 113 0 is_stmt 1
	movl	-20(%rbp), %eax
	movl	%eax, -48(%rbp)
	movss	-48(%rbp), %xmm0
	.loc 1 114 0
	popq	%rbp
.LCFI32:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	kahan_summationUp, .-kahan_summationUp
	.globl	kahan_summationDown
	.type	kahan_summationDown, @function
kahan_summationDown:
.LFB11:
	.loc 1 115 0
	.cfi_startproc
	pushq	%rbp
.LCFI33:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI34:
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	.loc 1 116 0
	movl	$0x00000000, %eax
	movl	%eax, -20(%rbp)
	.loc 1 118 0
	movl	$0x00000000, %eax
	movl	%eax, -16(%rbp)
.LBB11:
	.loc 1 119 0
	movl	-44(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -12(%rbp)
	jmp	.L29
.L30:
.LBB12:
	.loc 1 120 0 discriminator 2
	movl	-12(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	-40(%rbp), %rax
	movss	(%rax), %xmm0
	subss	-16(%rbp), %xmm0
	movss	%xmm0, -8(%rbp)
	.loc 1 121 0 discriminator 2
	movss	-20(%rbp), %xmm0
	addss	-8(%rbp), %xmm0
	movss	%xmm0, -4(%rbp)
	.loc 1 122 0 discriminator 2
	movss	-4(%rbp), %xmm0
	subss	-20(%rbp), %xmm0
	subss	-8(%rbp), %xmm0
	movss	%xmm0, -16(%rbp)
	.loc 1 123 0 discriminator 2
	movl	-4(%rbp), %eax
	movl	%eax, -20(%rbp)
.LBE12:
	.loc 1 119 0 discriminator 2
	subl	$1, -12(%rbp)
.L29:
	.loc 1 119 0 is_stmt 0 discriminator 1
	cmpl	$0, -12(%rbp)
	jns	.L30
.LBE11:
	.loc 1 125 0 is_stmt 1
	movl	-20(%rbp), %eax
	movl	%eax, -48(%rbp)
	movss	-48(%rbp), %xmm0
	.loc 1 126 0
	popq	%rbp
.LCFI35:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	kahan_summationDown, .-kahan_summationDown
	.globl	fsumRecursive
	.type	fsumRecursive, @function
fsumRecursive:
.LFB12:
	.loc 1 127 0
	.cfi_startproc
	pushq	%rbp
.LCFI36:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI37:
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	.loc 1 128 0
	cmpl	$0, -28(%rbp)
	jne	.L32
	.loc 1 128 0 is_stmt 0 discriminator 1
	xorps	%xmm0, %xmm0
	jmp	.L33
.L32:
	.loc 1 129 0 is_stmt 1
	cmpl	$1, -28(%rbp)
	jne	.L34
	.loc 1 129 0 is_stmt 0 discriminator 1
	movq	-24(%rbp), %rax
	movss	(%rax), %xmm0
	jmp	.L33
.L34:
.LBB13:
	.loc 1 131 0 is_stmt 1
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
	.loc 1 132 0
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
	.loc 1 133 0
	movss	-8(%rbp), %xmm0
	addss	-4(%rbp), %xmm0
.L33:
.LBE13:
	.loc 1 135 0
	leave
.LCFI38:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	fsumRecursive, .-fsumRecursive
	.globl	DoTheSummation
	.type	DoTheSummation, @function
DoTheSummation:
.LFB13:
	.loc 1 139 0
	.cfi_startproc
	pushq	%rbp
.LCFI39:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI40:
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -56(%rbp)
	movl	%esi, -60(%rbp)
	.loc 1 141 0
	movl	-60(%rbp), %edx
	movq	-56(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	fsumUp
	movss	%xmm0, -28(%rbp)
	.loc 1 142 0
	movl	-60(%rbp), %edx
	movq	-56(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	fsumUp2
	movss	%xmm0, -24(%rbp)
	.loc 1 143 0
	movl	-60(%rbp), %edx
	movq	-56(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	fsumDown
	movss	%xmm0, -20(%rbp)
	.loc 1 144 0
	movl	-60(%rbp), %edx
	movq	-56(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	dsumUp
	movsd	%xmm0, -48(%rbp)
	.loc 1 145 0
	movl	-60(%rbp), %edx
	movq	-56(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	dsumDown
	movsd	%xmm0, -40(%rbp)
	.loc 1 146 0
	movl	-60(%rbp), %edx
	movq	-56(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	fsumUp
	movss	%xmm0, -16(%rbp)
	.loc 1 147 0
	movl	-60(%rbp), %edx
	movq	-56(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	kahan_summationUp
	movss	%xmm0, -12(%rbp)
	.loc 1 148 0
	movl	-60(%rbp), %edx
	movq	-56(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	kahan_summationDown
	movss	%xmm0, -8(%rbp)
	.loc 1 149 0
	movl	-60(%rbp), %edx
	movq	-56(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	fsumRecursive
	movss	%xmm0, -4(%rbp)
	.loc 1 154 0
	leave
.LCFI41:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	DoTheSummation, .-DoTheSummation
	.globl	printItForDebug
	.type	printItForDebug, @function
printItForDebug:
.LFB14:
	.loc 1 157 0
	.cfi_startproc
	pushq	%rbp
.LCFI42:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI43:
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	.loc 1 163 0
	popq	%rbp
.LCFI44:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	printItForDebug, .-printItForDebug
	.section	.rodata
.LC4:
	.string	"Ones:      "
	.text
	.globl	main
	.type	main, @function
main:
.LFB15:
	.loc 1 165 0
	.cfi_startproc
	pushq	%rbp
.LCFI45:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI46:
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	.loc 1 168 0
	movl	$.LC4, %edi
	call	puts
	.loc 1 169 0
	movss	.LC1(%rip), %xmm1
	movss	.LC5(%rip), %xmm0
	movl	$16000000, %edx
	movl	$0, %esi
	movl	$A, %edi
	call	fillOnes
	.loc 1 170 0
	movl	$16000000, %esi
	movl	$A, %edi
	call	DoTheSummation
	.loc 1 171 0
	movl	$16000000, %esi
	movl	$A, %edi
	call	printItForDebug
	.loc 1 187 0
	movl	$0, %eax
	.loc 1 188 0
	leave
.LCFI47:
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
	.text
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x7c3
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF35
	.byte	0x1
	.long	.LASF36
	.long	.LASF37
	.quad	.Ltext0
	.quad	.Letext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF7
	.uleb128 0x4
	.byte	0x8
	.long	0x72
	.uleb128 0x5
	.long	0x65
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF8
	.uleb128 0x4
	.byte	0x8
	.long	0x84
	.uleb128 0x6
	.uleb128 0x7
	.byte	0x1
	.long	.LASF12
	.byte	0x1
	.byte	0x11
	.byte	0x1
	.long	0xc3
	.quad	.LFB0
	.quad	.LFE0
	.long	.LLST0
	.long	0xc3
	.uleb128 0x8
	.string	"a"
	.byte	0x1
	.byte	0x11
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x8
	.string	"b"
	.byte	0x1
	.byte	0x11
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x2
	.byte	0x4
	.byte	0x4
	.long	.LASF9
	.uleb128 0x9
	.byte	0x1
	.long	.LASF14
	.byte	0x1
	.byte	0x16
	.byte	0x1
	.quad	.LFB1
	.quad	.LFE1
	.long	.LLST1
	.long	0x14f
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x16
	.long	0x14f
	.byte	0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0xa
	.long	.LASF10
	.byte	0x1
	.byte	0x16
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -60
	.uleb128 0x8
	.string	"end"
	.byte	0x1
	.byte	0x16
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0xa
	.long	.LASF11
	.byte	0x1
	.byte	0x16
	.long	0xc3
	.byte	0x3
	.byte	0x91
	.sleb128 -68
	.uleb128 0x8
	.string	"to"
	.byte	0x1
	.byte	0x16
	.long	0xc3
	.byte	0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0xb
	.quad	.LBB2
	.quad	.LBE2
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x19
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -36
	.byte	0
	.byte	0
	.uleb128 0x4
	.byte	0x8
	.long	0xc3
	.uleb128 0x7
	.byte	0x1
	.long	.LASF13
	.byte	0x1
	.byte	0x1e
	.byte	0x1
	.long	0x57
	.quad	.LFB2
	.quad	.LFE2
	.long	.LLST2
	.long	0x1af
	.uleb128 0x8
	.string	"av"
	.byte	0x1
	.byte	0x1e
	.long	0x7e
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x8
	.string	"bv"
	.byte	0x1
	.byte	0x1e
	.long	0x7e
	.byte	0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0xc
	.string	"af"
	.byte	0x1
	.byte	0x1e
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xc
	.string	"bf"
	.byte	0x1
	.byte	0x1e
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x9
	.byte	0x1
	.long	.LASF15
	.byte	0x1
	.byte	0x1f
	.byte	0x1
	.quad	.LFB3
	.quad	.LFE3
	.long	.LLST3
	.long	0x214
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x1f
	.long	0x14f
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xa
	.long	.LASF10
	.byte	0x1
	.byte	0x1f
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x8
	.string	"end"
	.byte	0x1
	.byte	0x1f
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xa
	.long	.LASF11
	.byte	0x1
	.byte	0x1f
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x8
	.string	"to"
	.byte	0x1
	.byte	0x1f
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.byte	0
	.uleb128 0x9
	.byte	0x1
	.long	.LASF16
	.byte	0x1
	.byte	0x26
	.byte	0x1
	.quad	.LFB4
	.quad	.LFE4
	.long	.LLST4
	.long	0x297
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x26
	.long	0x14f
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xa
	.long	.LASF10
	.byte	0x1
	.byte	0x26
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x8
	.string	"end"
	.byte	0x1
	.byte	0x26
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0xa
	.long	.LASF11
	.byte	0x1
	.byte	0x26
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -52
	.uleb128 0x8
	.string	"to"
	.byte	0x1
	.byte	0x26
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0xb
	.quad	.LBB3
	.quad	.LBE3
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x28
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.uleb128 0x7
	.byte	0x1
	.long	.LASF17
	.byte	0x1
	.byte	0x2e
	.byte	0x1
	.long	0xc3
	.quad	.LFB5
	.quad	.LFE5
	.long	.LLST5
	.long	0x301
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x2e
	.long	0x14f
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xa
	.long	.LASF18
	.byte	0x1
	.byte	0x2e
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x32
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xb
	.quad	.LBB4
	.quad	.LBE4
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x35
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.uleb128 0x7
	.byte	0x1
	.long	.LASF19
	.byte	0x1
	.byte	0x3f
	.byte	0x1
	.long	0xc3
	.quad	.LFB6
	.quad	.LFE6
	.long	.LLST6
	.long	0x36b
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x3f
	.long	0x14f
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xa
	.long	.LASF18
	.byte	0x1
	.byte	0x3f
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x41
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xb
	.quad	.LBB5
	.quad	.LBE5
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x42
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.uleb128 0x7
	.byte	0x1
	.long	.LASF20
	.byte	0x1
	.byte	0x4a
	.byte	0x1
	.long	0xc3
	.quad	.LFB7
	.quad	.LFE7
	.long	.LLST7
	.long	0x3d5
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x4a
	.long	0x14f
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xa
	.long	.LASF18
	.byte	0x1
	.byte	0x4a
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x4c
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xb
	.quad	.LBB6
	.quad	.LBE6
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x4f
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.uleb128 0x7
	.byte	0x1
	.long	.LASF21
	.byte	0x1
	.byte	0x55
	.byte	0x1
	.long	0x43f
	.quad	.LFB8
	.quad	.LFE8
	.long	.LLST8
	.long	0x43f
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x55
	.long	0x14f
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xa
	.long	.LASF18
	.byte	0x1
	.byte	0x55
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x57
	.long	0x43f
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xb
	.quad	.LBB7
	.quad	.LBE7
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x58
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.long	.LASF22
	.uleb128 0x7
	.byte	0x1
	.long	.LASF23
	.byte	0x1
	.byte	0x5e
	.byte	0x1
	.long	0x43f
	.quad	.LFB9
	.quad	.LFE9
	.long	.LLST9
	.long	0x4b0
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x5e
	.long	0x14f
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xa
	.long	.LASF18
	.byte	0x1
	.byte	0x5e
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x60
	.long	0x43f
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xb
	.quad	.LBB8
	.quad	.LBE8
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x61
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.uleb128 0x7
	.byte	0x1
	.long	.LASF24
	.byte	0x1
	.byte	0x67
	.byte	0x1
	.long	0xc3
	.quad	.LFB10
	.quad	.LFE10
	.long	.LLST10
	.long	0x552
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x67
	.long	0x14f
	.byte	0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0xa
	.long	.LASF18
	.byte	0x1
	.byte	0x67
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -60
	.uleb128 0xd
	.long	.LASF25
	.byte	0x1
	.byte	0x68
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0xc
	.string	"c"
	.byte	0x1
	.byte	0x6a
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xb
	.quad	.LBB9
	.quad	.LBE9
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x6b
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0xb
	.quad	.LBB10
	.quad	.LBE10
	.uleb128 0xc
	.string	"y"
	.byte	0x1
	.byte	0x6c
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xc
	.string	"t"
	.byte	0x1
	.byte	0x6d
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x7
	.byte	0x1
	.long	.LASF26
	.byte	0x1
	.byte	0x73
	.byte	0x1
	.long	0xc3
	.quad	.LFB11
	.quad	.LFE11
	.long	.LLST11
	.long	0x5f4
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x73
	.long	0x14f
	.byte	0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0xa
	.long	.LASF18
	.byte	0x1
	.byte	0x73
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -60
	.uleb128 0xd
	.long	.LASF25
	.byte	0x1
	.byte	0x74
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0xc
	.string	"c"
	.byte	0x1
	.byte	0x76
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xb
	.quad	.LBB11
	.quad	.LBE11
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x77
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0xb
	.quad	.LBB12
	.quad	.LBE12
	.uleb128 0xc
	.string	"y"
	.byte	0x1
	.byte	0x78
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xc
	.string	"t"
	.byte	0x1
	.byte	0x79
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x7
	.byte	0x1
	.long	.LASF27
	.byte	0x1
	.byte	0x7f
	.byte	0x1
	.long	0xc3
	.quad	.LFB12
	.quad	.LFE12
	.long	.LLST12
	.long	0x660
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x7f
	.long	0x14f
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xa
	.long	.LASF18
	.byte	0x1
	.byte	0x7f
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0xb
	.quad	.LBB13
	.quad	.LBE13
	.uleb128 0xc
	.string	"r1"
	.byte	0x1
	.byte	0x83
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xc
	.string	"r2"
	.byte	0x1
	.byte	0x84
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.uleb128 0x9
	.byte	0x1
	.long	.LASF28
	.byte	0x1
	.byte	0x8a
	.byte	0x1
	.quad	.LFB13
	.quad	.LFE13
	.long	.LLST13
	.long	0x71b
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x8a
	.long	0x14f
	.byte	0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0xa
	.long	.LASF18
	.byte	0x1
	.byte	0x8a
	.long	0x57
	.byte	0x3
	.byte	0x91
	.sleb128 -76
	.uleb128 0xc
	.string	"fr1"
	.byte	0x1
	.byte	0x8d
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0xd
	.long	.LASF29
	.byte	0x1
	.byte	0x8e
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xc
	.string	"fr2"
	.byte	0x1
	.byte	0x8f
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0xc
	.string	"dr1"
	.byte	0x1
	.byte	0x90
	.long	0x43f
	.byte	0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0xc
	.string	"dr2"
	.byte	0x1
	.byte	0x91
	.long	0x43f
	.byte	0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0xd
	.long	.LASF30
	.byte	0x1
	.byte	0x92
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xc
	.string	"kr1"
	.byte	0x1
	.byte	0x93
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0xc
	.string	"kr2"
	.byte	0x1
	.byte	0x94
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xc
	.string	"rr"
	.byte	0x1
	.byte	0x95
	.long	0xc3
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x9
	.byte	0x1
	.long	.LASF31
	.byte	0x1
	.byte	0x9c
	.byte	0x1
	.quad	.LFB14
	.quad	.LFE14
	.long	.LLST14
	.long	0x757
	.uleb128 0x8
	.string	"A"
	.byte	0x1
	.byte	0x9c
	.long	0x14f
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xa
	.long	.LASF18
	.byte	0x1
	.byte	0x9c
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.uleb128 0x7
	.byte	0x1
	.long	.LASF32
	.byte	0x1
	.byte	0xa5
	.byte	0x1
	.long	0x57
	.quad	.LFB15
	.quad	.LFE15
	.long	.LLST15
	.long	0x799
	.uleb128 0xa
	.long	.LASF33
	.byte	0x1
	.byte	0xa5
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0xa
	.long	.LASF34
	.byte	0x1
	.byte	0xa5
	.long	0x799
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x4
	.byte	0x8
	.long	0x6c
	.uleb128 0xe
	.long	0xc3
	.long	0x7b2
	.uleb128 0xf
	.long	0x2d
	.long	0xf423ff
	.byte	0
	.uleb128 0x10
	.string	"A"
	.byte	0x1
	.byte	0xf
	.long	0x79f
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	A
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.quad	.LFB0-.Ltext0
	.quad	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI0-.Ltext0
	.quad	.LCFI1-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI1-.Ltext0
	.quad	.LCFI2-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI2-.Ltext0
	.quad	.LFE0-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST1:
	.quad	.LFB1-.Ltext0
	.quad	.LCFI3-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI3-.Ltext0
	.quad	.LCFI4-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI4-.Ltext0
	.quad	.LCFI5-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI5-.Ltext0
	.quad	.LFE1-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST2:
	.quad	.LFB2-.Ltext0
	.quad	.LCFI6-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI6-.Ltext0
	.quad	.LCFI7-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI7-.Ltext0
	.quad	.LCFI8-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI8-.Ltext0
	.quad	.LFE2-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST3:
	.quad	.LFB3-.Ltext0
	.quad	.LCFI9-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI9-.Ltext0
	.quad	.LCFI10-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI10-.Ltext0
	.quad	.LCFI11-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI11-.Ltext0
	.quad	.LFE3-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST4:
	.quad	.LFB4-.Ltext0
	.quad	.LCFI12-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI12-.Ltext0
	.quad	.LCFI13-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI13-.Ltext0
	.quad	.LCFI14-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI14-.Ltext0
	.quad	.LFE4-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST5:
	.quad	.LFB5-.Ltext0
	.quad	.LCFI15-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI15-.Ltext0
	.quad	.LCFI16-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI16-.Ltext0
	.quad	.LCFI17-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI17-.Ltext0
	.quad	.LFE5-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST6:
	.quad	.LFB6-.Ltext0
	.quad	.LCFI18-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI18-.Ltext0
	.quad	.LCFI19-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI19-.Ltext0
	.quad	.LCFI20-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI20-.Ltext0
	.quad	.LFE6-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST7:
	.quad	.LFB7-.Ltext0
	.quad	.LCFI21-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI21-.Ltext0
	.quad	.LCFI22-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI22-.Ltext0
	.quad	.LCFI23-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI23-.Ltext0
	.quad	.LFE7-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST8:
	.quad	.LFB8-.Ltext0
	.quad	.LCFI24-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI24-.Ltext0
	.quad	.LCFI25-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI25-.Ltext0
	.quad	.LCFI26-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI26-.Ltext0
	.quad	.LFE8-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST9:
	.quad	.LFB9-.Ltext0
	.quad	.LCFI27-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI27-.Ltext0
	.quad	.LCFI28-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI28-.Ltext0
	.quad	.LCFI29-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI29-.Ltext0
	.quad	.LFE9-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST10:
	.quad	.LFB10-.Ltext0
	.quad	.LCFI30-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI30-.Ltext0
	.quad	.LCFI31-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI31-.Ltext0
	.quad	.LCFI32-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI32-.Ltext0
	.quad	.LFE10-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST11:
	.quad	.LFB11-.Ltext0
	.quad	.LCFI33-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI33-.Ltext0
	.quad	.LCFI34-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI34-.Ltext0
	.quad	.LCFI35-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI35-.Ltext0
	.quad	.LFE11-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST12:
	.quad	.LFB12-.Ltext0
	.quad	.LCFI36-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI36-.Ltext0
	.quad	.LCFI37-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI37-.Ltext0
	.quad	.LCFI38-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI38-.Ltext0
	.quad	.LFE12-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST13:
	.quad	.LFB13-.Ltext0
	.quad	.LCFI39-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI39-.Ltext0
	.quad	.LCFI40-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI40-.Ltext0
	.quad	.LCFI41-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI41-.Ltext0
	.quad	.LFE13-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST14:
	.quad	.LFB14-.Ltext0
	.quad	.LCFI42-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI42-.Ltext0
	.quad	.LCFI43-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI43-.Ltext0
	.quad	.LCFI44-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI44-.Ltext0
	.quad	.LFE14-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
.LLST15:
	.quad	.LFB15-.Ltext0
	.quad	.LCFI45-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI45-.Ltext0
	.quad	.LCFI46-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI46-.Ltext0
	.quad	.LCFI47-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	.LCFI47-.Ltext0
	.quad	.LFE15-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	0
	.quad	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF11:
	.string	"from"
.LASF37:
	.string	"/homes/za1610/indProject/DynamoRIO/test2"
.LASF31:
	.string	"printItForDebug"
.LASF19:
	.string	"fsumUp2"
.LASF29:
	.string	"ffr1"
.LASF9:
	.string	"float"
.LASF18:
	.string	"size"
.LASF30:
	.string	"fffr1"
.LASF0:
	.string	"long unsigned int"
.LASF2:
	.string	"short unsigned int"
.LASF21:
	.string	"dsumUp"
.LASF20:
	.string	"fsumDown"
.LASF35:
	.string	"GNU C 4.6.3"
.LASF1:
	.string	"unsigned char"
.LASF26:
	.string	"kahan_summationDown"
.LASF22:
	.string	"double"
.LASF32:
	.string	"main"
.LASF15:
	.string	"fillRandSorted"
.LASF27:
	.string	"fsumRecursive"
.LASF3:
	.string	"unsigned int"
.LASF10:
	.string	"start"
.LASF25:
	.string	"result"
.LASF33:
	.string	"argc"
.LASF24:
	.string	"kahan_summationUp"
.LASF8:
	.string	"long long int"
.LASF7:
	.string	"char"
.LASF17:
	.string	"fsumUp"
.LASF5:
	.string	"short int"
.LASF34:
	.string	"argv"
.LASF14:
	.string	"fillRand"
.LASF12:
	.string	"rand_FloatRange"
.LASF6:
	.string	"long int"
.LASF23:
	.string	"dsumDown"
.LASF28:
	.string	"DoTheSummation"
.LASF4:
	.string	"signed char"
.LASF13:
	.string	"compare"
.LASF16:
	.string	"fillOnes"
.LASF36:
	.string	"m2.c"
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
