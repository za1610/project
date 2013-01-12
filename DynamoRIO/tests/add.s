	.file	"add.c"
	.text
.globl Add1
	.type	Add1, @function
Add1:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	movss	%xmm0, -4(%rbp)
	movss	-4(%rbp), %xmm1
	movss	.LC0(%rip), %xmm0
	addss	%xmm1, %xmm0
	leave
	ret
	.cfi_endproc
.LFE0:
	.size	Add1, .-Add1
.globl Add1Again
	.type	Add1Again, @function
Add1Again:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	movss	%xmm0, -4(%rbp)
	movq	%rdi, -16(%rbp)
	movq	-16(%rbp), %rax
	movss	(%rax), %xmm0
	addss	-4(%rbp), %xmm0
	movq	-16(%rbp), %rax
	movss	%xmm0, (%rax)
	leave
	ret
	.cfi_endproc
.LFE1:
	.size	Add1Again, .-Add1Again
.globl p
	.data
	.align 4
	.type	p, @object
	.size	p, 4
p:
	.long	1092745631
.globl y
	.align 4
	.type	y, @object
	.size	y, 4
y:
	.long	1039932378
.globl z
	.bss
	.align 4
	.type	z, @object
	.size	z, 4
z:
	.zero	4
	.section	.rodata
.LC1:
	.string	"p = %f\t\ty = %f\n"
.LC2:
	.string	"p = %f\t\tz = %f\n"
	.text
.globl main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	movss	p(%rip), %xmm0
	call	Add1
	movss	%xmm0, y(%rip)
	movss	y(%rip), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm1
	movss	p(%rip), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC1, %eax
	movq	%rax, %rdi
	movl	$2, %eax
	call	printf
	movss	p(%rip), %xmm0
	movl	$z, %edi
	call	Add1Again
	movss	z(%rip), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm1
	movss	p(%rip), %xmm0
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC2, %eax
	movq	%rax, %rdi
	movl	$2, %eax
	call	printf
	leave
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1065353216
	.ident	"GCC: (Ubuntu 4.4.3-4ubuntu5) 4.4.3"
	.section	.note.GNU-stack,"",@progbits
