	.file	"mul.c"
	.intel_syntax noprefix
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
	mov	eax, 0x40266666
	mov	DWORD PTR [rbp-12], eax
	mov	eax, 0x4059999a
	mov	DWORD PTR [rbp-8], eax
	movss	xmm0, DWORD PTR [rbp-12]
	movaps	xmm1, xmm0
	mulss	xmm1, DWORD PTR [rbp-12]
	movss	xmm0, DWORD PTR [rbp-8]
	mulss	xmm0, DWORD PTR [rbp-8]
	addss	xmm0, xmm1
	movss	DWORD PTR [rbp-4], xmm0
	mov	eax, 0
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
