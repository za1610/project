	.file	"add.c"
	.intel_syntax noprefix
	.text
.Ltext0:
	.globl	Add1
	.type	Add1, @function
Add1:
.LFB0:
	.file 1 "add.c"
	.loc 1 3 0
	.cfi_startproc
	push	rbp
.LCFI0:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
.LCFI1:
	.cfi_def_cfa_register 6
	movss	DWORD PTR [rbp-4], xmm0
	movss	DWORD PTR [rbp-8], xmm1
	.loc 1 3 0
	movss	xmm0, DWORD PTR [rbp-4]
	addss	xmm0, DWORD PTR [rbp-8]
	pop	rbp
.LCFI2:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	Add1, .-Add1
	.globl	p
	.data
	.align 4
	.type	p, @object
	.size	p, 4
p:
	.long	1092745167
	.globl	y
	.align 4
	.type	y, @object
	.size	y, 4
y:
	.long	1039918957
	.globl	z
	.bss
	.align 4
	.type	z, @object
	.size	z, 4
z:
	.zero	4
	.section	.rodata
.LC0:
	.string	"p = %f\t\ty = %f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.loc 1 11 0
	.cfi_startproc
	push	rbp
.LCFI3:
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
.LCFI4:
	.cfi_def_cfa_register 6
	sub	rsp, 16
	.loc 1 13 0
	mov	DWORD PTR [rbp-4], 0
	jmp	.L3
.L4:
	.loc 1 14 0 discriminator 2
	movss	xmm1, DWORD PTR p[rip]
	movss	xmm0, DWORD PTR y[rip]
	addss	xmm0, xmm1
	movss	DWORD PTR p[rip], xmm0
	.loc 1 13 0 discriminator 2
	add	DWORD PTR [rbp-4], 1
.L3:
	.loc 1 13 0 is_stmt 0 discriminator 1
	cmp	DWORD PTR [rbp-4], 9
	jle	.L4
	.loc 1 17 0 is_stmt 1
	movss	xmm0, DWORD PTR y[rip]
	unpcklps	xmm0, xmm0
	cvtps2pd	xmm1, xmm0
	movss	xmm0, DWORD PTR p[rip]
	unpcklps	xmm0, xmm0
	cvtps2pd	xmm0, xmm0
	mov	eax, OFFSET FLAT:.LC0
	mov	rdi, rax
	mov	eax, 2
	call	printf
	.loc 1 22 0
	leave
.LCFI5:
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x117
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF9
	.byte	0x1
	.long	.LASF10
	.long	.LASF11
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
	.byte	0x1
	.long	.LASF12
	.byte	0x1
	.byte	0x3
	.byte	0x1
	.long	0xaa
	.quad	.LFB0
	.quad	.LFE0
	.long	.LLST0
	.long	0xaa
	.uleb128 0x5
	.string	"i"
	.byte	0x1
	.byte	0x3
	.long	0xaa
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x5
	.string	"j"
	.byte	0x1
	.byte	0x3
	.long	0xaa
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x2
	.byte	0x4
	.byte	0x4
	.long	.LASF8
	.uleb128 0x6
	.byte	0x1
	.long	.LASF13
	.byte	0x1
	.byte	0xa
	.quad	.LFB1
	.quad	.LFE1
	.long	.LLST1
	.long	0xde
	.uleb128 0x7
	.string	"i"
	.byte	0x1
	.byte	0xc
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x8
	.string	"p"
	.byte	0x1
	.byte	0x6
	.long	0xaa
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	p
	.uleb128 0x8
	.string	"y"
	.byte	0x1
	.byte	0x7
	.long	0xaa
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	y
	.uleb128 0x8
	.string	"z"
	.byte	0x1
	.byte	0x8
	.long	0xaa
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	z
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
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
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
	.uleb128 0x6
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
	.uleb128 0x7
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
	.uleb128 0x8
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
.LASF0:
	.string	"long unsigned int"
.LASF12:
	.string	"Add1"
.LASF1:
	.string	"unsigned char"
.LASF10:
	.string	"add.c"
.LASF13:
	.string	"main"
.LASF6:
	.string	"long int"
.LASF9:
	.string	"GNU C 4.6.3"
.LASF11:
	.string	"/homes/za1610/indProject/DynamoRIO/tests"
.LASF2:
	.string	"short unsigned int"
.LASF4:
	.string	"signed char"
.LASF8:
	.string	"float"
.LASF5:
	.string	"short int"
.LASF7:
	.string	"char"
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
