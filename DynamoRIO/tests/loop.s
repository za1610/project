	.file	"loop.c"
	.intel_syntax noprefix
	.section	.rodata
.LC3:
	.string	"Result id %f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	DWORD PTR [rbp-12], 0
	mov	DWORD PTR [rbp-12], 0
	jmp	.L2
.L3:
	mov	eax, 0x4083ef9e
	mov	DWORD PTR [rbp-8], eax
	mov	eax, 0x4015c28f
	mov	DWORD PTR [rbp-4], eax
	movss	xmm0, DWORD PTR [rbp-8]
	cvtps2pd	xmm0, xmm0
	movsd	xmm1, QWORD PTR .LC2[rip]
	addsd	xmm0, xmm1
	unpcklpd	xmm0, xmm0
	cvtpd2ps	xmm0, xmm0
	movss	DWORD PTR [rbp-16], xmm0
	add	DWORD PTR [rbp-12], 1
.L2:
	cmp	DWORD PTR [rbp-12], 2
	jle	.L3
	movss	xmm0, DWORD PTR [rbp-16]
	cvtps2pd	xmm0, xmm0
	mov	edi, OFFSET FLAT:.LC3
	mov	eax, 1
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
