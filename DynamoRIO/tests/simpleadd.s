	.file	"simpleadd.c"
	.section	.rodata
.LC3:
	.string	"Result id %f\n"
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
	subq	$16, %rsp
	movl	$0x4083ef9e, %eax
	movl	%eax, -12(%rbp)
	movl	$0x4015c28f, %eax
	movl	%eax, -8(%rbp)
	movss	-12(%rbp), %xmm12
	cvtps2pd	%xmm12, %xmm12
	movsd	.LC2(%rip), %xmm1
	addsd	%xmm1, %xmm12
	unpcklpd	%xmm12, %xmm12
	cvtpd2ps	%xmm12, %xmm12
	movss	%xmm12, -4(%rbp)
	movss	-4(%rbp), %xmm12
	cvtps2pd	%xmm12, %xmm12
	movl	$.LC3, %edi
	movl	$1, %eax
	call	printf
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC2:
	.long	1281535778
	.long	1073042773
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
