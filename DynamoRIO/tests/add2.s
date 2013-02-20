	.file	"add2.c"
	.text
.Ltext0:
	.section	.rodata
.LC1:
	.string	"e %.13f\n"
.LC3:
	.string	"x %.13f\n"
.LC5:
	.string	"y %.13f\n"
.LC6:
	.string	"more %.13f\n"
.LC7:
	.string	"diff_e %.13f\n"
.LC8:
	.string	"diff_o %.13f\n"
.LC9:
	.string	"!!!!!!!!! %.13lf\n"
.LC10:
	.string	"!!!!!!!!! diff %.13lf\n"
.LC11:
	.string	"zero %.13f\n"
.LC12:
	.string	"result %.13f\n"
	.text
	.globl	addFloats
	.type	addFloats, @function
addFloats:
.LFB0:
	.file 1 "add2.c"
	.loc 1 4 0
	.cfi_startproc
	pushq	%rbp
.LCFI0:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI1:
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	.loc 1 5 0
	movl	$0x3380d959, %eax
	movl	%eax, -24(%rbp)
	.loc 1 6 0
	movss	-24(%rbp), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC1, %eax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	.loc 1 7 0
	movl	$0x3f000000, %eax
	movl	%eax, -20(%rbp)
	.loc 1 8 0
	movss	-20(%rbp), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC3, %eax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	.loc 1 9 0
	movss	-20(%rbp), %xmm1
	movss	.LC4(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -16(%rbp)
	.loc 1 10 0
	movss	-16(%rbp), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC5, %eax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	.loc 1 11 0
	movss	-16(%rbp), %xmm0
	addss	-24(%rbp), %xmm0
	movss	%xmm0, -36(%rbp)
	.loc 1 12 0
	movss	-36(%rbp), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC6, %eax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	.loc 1 15 0
	movl	$0, -32(%rbp)
	jmp	.L2
.L3:
	.loc 1 16 0 discriminator 2
	movss	-36(%rbp), %xmm0
	subss	-16(%rbp), %xmm0
	movss	%xmm0, -28(%rbp)
	.loc 1 17 0 discriminator 2
	movss	-36(%rbp), %xmm0
	addss	-24(%rbp), %xmm0
	movss	%xmm0, -36(%rbp)
	.loc 1 15 0 discriminator 2
	addl	$1, -32(%rbp)
.L2:
	.loc 1 15 0 is_stmt 0 discriminator 1
	cmpl	$9, -32(%rbp)
	jle	.L3
	.loc 1 19 0 is_stmt 1
	movss	-28(%rbp), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC7, %eax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	.loc 1 22 0
	movss	-28(%rbp), %xmm0
	subss	-24(%rbp), %xmm0
	movss	%xmm0, -12(%rbp)
	.loc 1 23 0
	movss	-12(%rbp), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC8, %eax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	.loc 1 24 0
	movss	-28(%rbp), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movsd	%xmm0, -72(%rbp)
	.loc 1 25 0
	movss	-24(%rbp), %xmm2
	cvtps2pd	%xmm2, %xmm2
	movsd	%xmm2, -64(%rbp)
	.loc 1 26 0
	movsd	-72(%rbp), %xmm0
	subsd	-64(%rbp), %xmm0
	movsd	%xmm0, -56(%rbp)
	.loc 1 27 0
	movss	-12(%rbp), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movsd	-56(%rbp), %xmm1
	movapd	%xmm1, %xmm2
	subsd	%xmm0, %xmm2
	movapd	%xmm2, %xmm0
	movsd	%xmm0, -48(%rbp)
	.loc 1 28 0
	movl	$.LC9, %eax
	movsd	-56(%rbp), %xmm0
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	.loc 1 29 0
	movl	$.LC10, %eax
	movsd	-48(%rbp), %xmm0
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	.loc 1 32 0
	movss	-12(%rbp), %xmm0
	addss	%xmm0, %xmm0
	movss	%xmm0, -8(%rbp)
	.loc 1 33 0
	movss	-8(%rbp), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC11, %eax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	.loc 1 34 0
	movss	-8(%rbp), %xmm0
	addss	%xmm0, %xmm0
	movss	%xmm0, -4(%rbp)
	.loc 1 35 0
	movss	-4(%rbp), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movl	$.LC12, %eax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	.loc 1 37 0
	leave
.LCFI2:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	addFloats, .-addFloats
	.globl	main
	.type	main, @function
main:
.LFB1:
	.loc 1 57 0
	.cfi_startproc
	pushq	%rbp
.LCFI3:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI4:
	.cfi_def_cfa_register 6
	.loc 1 58 0
	movl	$0, %eax
	call	addFloats
	.loc 1 134 0
	popq	%rbp
.LCFI5:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC4:
	.long	1065353216
	.text
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x161
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF15
	.byte	0x1
	.long	.LASF16
	.long	.LASF17
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
	.uleb128 0x2
	.byte	0x4
	.byte	0x4
	.long	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.long	.LASF9
	.uleb128 0x4
	.byte	0x1
	.long	.LASF18
	.byte	0x1
	.byte	0x4
	.quad	.LFB0
	.quad	.LFE0
	.long	.LLST0
	.long	0x148
	.uleb128 0x5
	.string	"e"
	.byte	0x1
	.byte	0x5
	.long	0x6c
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x5
	.string	"x"
	.byte	0x1
	.byte	0x7
	.long	0x6c
	.byte	0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x5
	.string	"y"
	.byte	0x1
	.byte	0x9
	.long	0x6c
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x6
	.long	.LASF10
	.byte	0x1
	.byte	0xb
	.long	0x6c
	.byte	0x2
	.byte	0x91
	.sleb128 -52
	.uleb128 0x5
	.string	"i"
	.byte	0x1
	.byte	0xd
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x6
	.long	.LASF11
	.byte	0x1
	.byte	0xe
	.long	0x6c
	.byte	0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x6
	.long	.LASF12
	.byte	0x1
	.byte	0x14
	.long	0x6c
	.byte	0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x5
	.string	"dd"
	.byte	0x1
	.byte	0x18
	.long	0x73
	.byte	0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0x5
	.string	"de"
	.byte	0x1
	.byte	0x19
	.long	0x73
	.byte	0x3
	.byte	0x91
	.sleb128 -80
	.uleb128 0x5
	.string	"res"
	.byte	0x1
	.byte	0x1a
	.long	0x73
	.byte	0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0x5
	.string	"r"
	.byte	0x1
	.byte	0x1b
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x6
	.long	.LASF13
	.byte	0x1
	.byte	0x20
	.long	0x6c
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x6
	.long	.LASF14
	.byte	0x1
	.byte	0x22
	.long	0x6c
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x7
	.byte	0x1
	.long	.LASF19
	.byte	0x1
	.byte	0x38
	.quad	.LFB1
	.quad	.LFE1
	.long	.LLST1
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
	.uleb128 0x5
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
	.uleb128 0x6
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
	.uleb128 0x7
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
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
.LASF3:
	.string	"unsigned int"
.LASF15:
	.string	"GNU C 4.6.3"
.LASF0:
	.string	"long unsigned int"
.LASF12:
	.string	"diff_o"
.LASF10:
	.string	"more"
.LASF1:
	.string	"unsigned char"
.LASF19:
	.string	"main"
.LASF14:
	.string	"result"
.LASF18:
	.string	"addFloats"
.LASF9:
	.string	"double"
.LASF6:
	.string	"long int"
.LASF17:
	.string	"/homes/za1610/indProject/DynamoRIO/tests"
.LASF2:
	.string	"short unsigned int"
.LASF4:
	.string	"signed char"
.LASF16:
	.string	"add2.c"
.LASF8:
	.string	"float"
.LASF13:
	.string	"zero"
.LASF5:
	.string	"short int"
.LASF11:
	.string	"diff_e"
.LASF7:
	.string	"char"
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
