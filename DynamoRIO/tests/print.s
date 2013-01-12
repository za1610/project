	.file	"print.c"
	.section	.rodata
.LC3:
	.string	"HIIII"
	.text
.globl main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movabsq	$-8301034833169297408, %rax
	movl	$16383, %edx
	movq	%rax, -16(%rbp)
	movl	%edx, -8(%rbp)
	movabsq	$-7378697629483821056, %rax
	movl	$16384, %edx
	movq	%rax, -32(%rbp)
	movl	%edx, -24(%rbp)
	fldt	-16(%rbp)
	fldt	-32(%rbp)
	fsubrp	%st, %st(1)
	fldt	.LC2(%rip)
	fxch	%st(1)
	fucomip	%st(1), %st
	fstp	%st(0)
	jp	.L6
	je	.L2
.L6:
.L5:
	movl	$.LC3, %edi
	call	puts
.L2:
	movl	$0, %eax
	leave
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 16
.LC2:
	.long	0
	.long	3983582208
	.long	16382
	.long	0
	.ident	"GCC: (Ubuntu 4.4.3-4ubuntu5) 4.4.3"
	.section	.note.GNU-stack,"",@progbits
