	.file	"sqrt.c"
	.text
.Ltext0:
	.section	.rodata
	.align 8
.LC3:
	.string	"2 raised to the power of 3 is %f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.file 1 "sqrt.c"
	.loc 1 11 0
	.cfi_startproc
	pushq	%rbp
.LCFI0:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
.LCFI1:
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.loc 1 15 0
	movl	$0x3f800000, %eax
	movl	%eax, -12(%rbp)
	.loc 1 17 0
	movl	$0, -8(%rbp)
	jmp	.L2
.L3:
	.loc 1 18 0 discriminator 2
	movss	-12(%rbp), %xmm1
	movss	.LC0(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -12(%rbp)
	.loc 1 17 0 discriminator 2
	addl	$1, -8(%rbp)
.L2:
	.loc 1 17 0 is_stmt 0 discriminator 1
	cmpl	$9, -8(%rbp)
	jle	.L3
	.loc 1 20 0 is_stmt 1
	movss	-12(%rbp), %xmm1
	movss	.LC0(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, -4(%rbp)
	.loc 1 22 0
	movl	-12(%rbp), %eax
	movl	%eax, -4(%rbp)
	movss	-12(%rbp), %xmm1
	movss	.LC0(%rip), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -12(%rbp)
	.loc 1 26 0
	movabsq	$4612924508324914790, %rax
	movq	%rax, -32(%rbp)
	.loc 1 27 0
	movabsq	$4611686018427387904, %rax
	movq	%rax, -24(%rbp)
	.loc 1 29 0
	movl	$0, -8(%rbp)
	jmp	.L4
.L5:
	.loc 1 30 0 discriminator 2
	movsd	-32(%rbp), %xmm0
	mulsd	-32(%rbp), %xmm0
	movsd	-40(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -40(%rbp)
	.loc 1 29 0 discriminator 2
	addl	$1, -8(%rbp)
.L4:
	.loc 1 29 0 is_stmt 0 discriminator 1
	cmpl	$999, -8(%rbp)
	jle	.L5
	.loc 1 31 0 is_stmt 1
	movl	$.LC3, %eax
	movsd	.LC4(%rip), %xmm0
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	.loc 1 32 0
	movl	$0, %eax
	.loc 1 33 0
	leave
.LCFI2:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1065353216
	.align 8
.LC4:
	.long	2080342824
	.long	1076081348
	.text
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0xe1
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF10
	.byte	0x1
	.long	.LASF11
	.long	.LASF12
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
	.long	.LASF13
	.byte	0x1
	.byte	0x9
	.byte	0x1
	.long	0x57
	.quad	.LFB0
	.quad	.LFE0
	.long	.LLST0
	.uleb128 0x5
	.string	"i"
	.byte	0x1
	.byte	0xf
	.long	0x6c
	.byte	0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x5
	.string	"j"
	.byte	0x1
	.byte	0xf
	.long	0x6c
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x5
	.string	"z"
	.byte	0x1
	.byte	0x10
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x5
	.string	"d"
	.byte	0x1
	.byte	0x1a
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x5
	.string	"k"
	.byte	0x1
	.byte	0x1b
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x5
	.string	"c"
	.byte	0x1
	.byte	0x1c
	.long	0x73
	.byte	0x2
	.byte	0x91
	.sleb128 -56
	.byte	0
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
.LASF2:
	.string	"short unsigned int"
.LASF3:
	.string	"unsigned int"
.LASF0:
	.string	"long unsigned int"
.LASF7:
	.string	"char"
.LASF11:
	.string	"sqrt.c"
.LASF1:
	.string	"unsigned char"
.LASF13:
	.string	"main"
.LASF6:
	.string	"long int"
.LASF9:
	.string	"double"
.LASF10:
	.string	"GNU C 4.6.3"
.LASF12:
	.string	"/homes/za1610/indProject/DynamoRIO/tests"
.LASF8:
	.string	"float"
.LASF5:
	.string	"short int"
.LASF4:
	.string	"signed char"
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
