	.file	"print.c"
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
	movabs	rax, -8301034833169297408
	mov	edx, 16383
	mov	QWORD PTR [rbp-48], rax
	mov	DWORD PTR [rbp-40], edx
	movabs	rax, -7378697629483821056
	mov	edx, 16384
	mov	QWORD PTR [rbp-32], rax
	mov	DWORD PTR [rbp-24], edx
	fld	TBYTE PTR [rbp-48]
	fld	TBYTE PTR .LC2[rip]
	faddp	st(1), st
	fstp	TBYTE PTR [rbp-16]
	mov	eax, 0
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 16
.LC2:
	.long	3951370240
	.long	2501818449
	.long	16384
	.long	0
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
