	.file	"main.cpp"
	.text
	.section	.text._ZNKSt5ctypeIcE8do_widenEc,"axG",@progbits,_ZNKSt5ctypeIcE8do_widenEc,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZNKSt5ctypeIcE8do_widenEc
	.type	_ZNKSt5ctypeIcE8do_widenEc, @function
_ZNKSt5ctypeIcE8do_widenEc:
.LFB1364:
	.cfi_startproc
	movl	%esi, %eax
	ret
	.cfi_endproc
.LFE1364:
	.size	_ZNKSt5ctypeIcE8do_widenEc, .-_ZNKSt5ctypeIcE8do_widenEc
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"/sys/class/net/wlo1/statistics/tx_packets"
	.align 8
.LC1:
	.string	"basic_string::_M_construct null not valid"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"stoull"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB1778:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA1778
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	movl	$24, %esi
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$1080, %rsp
	.cfi_def_cfa_offset 1136
	leaq	144(%rsp), %r15
	leaq	544(%rsp), %rbp
	movq	%fs:40, %rax
	movq	%rax, 1064(%rsp)
	xorl	%eax, %eax
	movq	%r15, %rdi
.LEHB0:
	call	_ZNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEEC1ESt13_Ios_Openmode@PLT
.LEHE0:
	movq	%rbp, %rdi
.LEHB1:
	call	_ZNSt14basic_ifstreamIcSt11char_traitsIcEEC1Ev@PLT
.LEHE1:
	leaq	48(%rsp), %rax
	leaq	64(%rsp), %r14
	movq	%rax, 8(%rsp)
	leaq	80(%rsp), %rax
	movq	%rax, (%rsp)
	.p2align 4,,10
	.p2align 3
.L51:
	movl	$1, %edi
.LEHB2:
	call	sleep@PLT
.L96:
	leaq	16(%rbp), %rdi
	leaq	.LC0(%rip), %rsi
	movl	$8, %edx
	call	_ZNSt13basic_filebufIcSt11char_traitsIcEE4openEPKcSt13_Ios_Openmode@PLT
	movq	544(%rsp), %rdx
	movq	-24(%rdx), %rdi
	addq	%rbp, %rdi
	testq	%rax, %rax
	je	.L88
	xorl	%esi, %esi
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate@PLT
.L6:
	leaq	16(%rbp), %rsi
	leaq	16(%r15), %rdi
	call	_ZNSolsEPSt15basic_streambufIcSt11char_traitsIcEE@PLT
.LEHE2:
	movq	208(%rsp), %rax
	movq	%r14, 48(%rsp)
	movq	$0, 56(%rsp)
	movb	$0, 64(%rsp)
	testq	%rax, %rax
	je	.L7
	movq	192(%rsp), %rbx
	movq	200(%rsp), %r12
	cmpq	%rbx, %rax
	jbe	.L8
	leaq	112(%rsp), %r13
	testq	%r12, %r12
	leaq	16(%r13), %rdx
	movq	%rdx, 112(%rsp)
	je	.L89
	subq	%r12, %rax
	cmpq	$15, %rax
	movq	%rax, %rbx
	movq	%rax, 24(%rsp)
	ja	.L90
	cmpq	$1, %rax
	jne	.L12
	movzbl	(%r12), %eax
	movb	%al, 128(%rsp)
.L13:
	movq	%rbx, 120(%rsp)
	movb	$0, (%rdx,%rbx)
	leaq	16(%r13), %rax
	movq	112(%rsp), %rsi
	movq	48(%rsp), %rdi
	movq	120(%rsp), %rdx
	cmpq	%rax, %rsi
	je	.L91
	cmpq	%r14, %rdi
	movq	128(%rsp), %rax
	je	.L92
	testq	%rdi, %rdi
	movq	64(%rsp), %rcx
	movq	%rsi, 48(%rsp)
	movq	%rdx, 56(%rsp)
	movq	%rax, 64(%rsp)
	je	.L19
	movq	%rdi, 112(%rsp)
	movq	%rcx, 128(%rsp)
.L17:
	movq	$0, 120(%rsp)
	movb	$0, (%rdi)
	addq	$16, %r13
	movq	112(%rsp), %rdi
	cmpq	%r13, %rdi
	je	.L21
.L85:
	call	_ZdlPv@PLT
.L21:
	movq	48(%rsp), %r12
	call	__errno_location@PLT
	leaq	40(%rsp), %rsi
	movl	(%rax), %r13d
	movl	$10, %edx
	movl	$0, (%rax)
	movq	%r12, %rdi
	movq	%rax, %rbx
	call	strtoull@PLT
	cmpq	40(%rsp), %r12
	je	.L93
	movl	(%rbx), %edx
	cmpl	$34, %edx
	je	.L94
	testl	%edx, %edx
	jne	.L39
	movl	%r13d, (%rbx)
.L39:
	leaq	_ZSt4cout(%rip), %rdi
	movq	%rax, %rsi
.LEHB3:
	call	_ZNSo9_M_insertIyEERSoT_@PLT
	movq	%rax, %r12
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%r12,%rax), %rbx
	testq	%rbx, %rbx
	je	.L95
	cmpb	$0, 56(%rbx)
	je	.L45
	movsbl	67(%rbx), %esi
.L46:
	movq	%r12, %rdi
	call	_ZNSo3putEc@PLT
	movq	%rax, %rdi
	call	_ZNSo5flushEv@PLT
	leaq	16(%rbp), %rdi
	call	_ZNSt13basic_filebufIcSt11char_traitsIcEE5closeEv@PLT
	testq	%rax, %rax
	je	.L47
.L49:
	leaq	128(%r15), %rdi
	xorl	%esi, %esi
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate@PLT
.LEHE3:
	movq	48(%rsp), %rdi
	cmpq	%r14, %rdi
	je	.L51
	call	_ZdlPv@PLT
	movl	$1, %edi
.LEHB4:
	call	sleep@PLT
.LEHE4:
	jmp	.L96
	.p2align 4,,10
	.p2align 3
.L8:
	movq	(%rsp), %rax
	addq	$16, %rax
	testq	%rbx, %rbx
	movq	%rax, 80(%rsp)
	je	.L22
	testq	%r12, %r12
	je	.L97
.L22:
	subq	%r12, %rbx
	cmpq	$15, %rbx
	movq	%rbx, 32(%rsp)
	ja	.L98
	cmpq	$1, %rbx
	jne	.L25
	movzbl	(%r12), %eax
	movb	%al, 96(%rsp)
	movq	(%rsp), %rax
	addq	$16, %rax
.L26:
	movq	%rbx, 88(%rsp)
	movb	$0, (%rax,%rbx)
	movq	(%rsp), %rax
	movq	80(%rsp), %rsi
	movq	48(%rsp), %rdi
	movq	88(%rsp), %rdx
	addq	$16, %rax
	cmpq	%rax, %rsi
	je	.L99
	cmpq	%r14, %rdi
	movq	96(%rsp), %rax
	je	.L100
	testq	%rdi, %rdi
	movq	64(%rsp), %rcx
	movq	%rsi, 48(%rsp)
	movq	%rdx, 56(%rsp)
	movq	%rax, 64(%rsp)
	je	.L32
	movq	(%rsp), %rax
	movq	%rdi, 80(%rsp)
	movq	%rcx, 96(%rsp)
.L30:
	movq	$0, 88(%rsp)
	movb	$0, (%rdi)
	addq	$16, %rax
	movq	80(%rsp), %rdi
	cmpq	%rax, %rdi
	jne	.L85
	jmp	.L21
	.p2align 4,,10
	.p2align 3
.L45:
	movq	%rbx, %rdi
.LEHB5:
	call	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
	movq	(%rbx), %rax
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rcx
	movl	$10, %esi
	movq	48(%rax), %rax
	cmpq	%rcx, %rax
	je	.L46
	movq	%rbx, %rdi
	call	*%rax
.LEHE5:
	movsbl	%al, %esi
	jmp	.L46
	.p2align 4,,10
	.p2align 3
.L88:
	movl	32(%rdi), %esi
	orl	$4, %esi
.LEHB6:
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate@PLT
.LEHE6:
	jmp	.L6
	.p2align 4,,10
	.p2align 3
.L47:
	movq	544(%rsp), %rax
	movq	-24(%rax), %rdi
	addq	%rbp, %rdi
	movl	32(%rdi), %esi
	orl	$4, %esi
.LEHB7:
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate@PLT
.LEHE7:
	jmp	.L49
	.p2align 4,,10
	.p2align 3
.L7:
	movq	8(%rsp), %rdi
	leaq	96(%r15), %rsi
.LEHB8:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@PLT
	jmp	.L21
	.p2align 4,,10
	.p2align 3
.L90:
	leaq	24(%rsp), %rsi
	xorl	%edx, %edx
	movq	%r13, %rdi
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@PLT
	movq	24(%rsp), %rdx
	movq	%rax, 112(%rsp)
	movq	%rdx, 128(%rsp)
.L11:
	movq	%rbx, %rdx
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	memcpy@PLT
	movq	24(%rsp), %rbx
	movq	112(%rsp), %rdx
	jmp	.L13
	.p2align 4,,10
	.p2align 3
.L98:
	movq	(%rsp), %rdi
	leaq	32(%rsp), %rsi
	xorl	%edx, %edx
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@PLT
.LEHE8:
	movq	32(%rsp), %rdx
	movq	%rax, 80(%rsp)
	movq	%rdx, 96(%rsp)
.L24:
	movq	%rbx, %rdx
	movq	%r12, %rsi
	movq	%rax, %rdi
	call	memcpy@PLT
	movq	32(%rsp), %rbx
	movq	80(%rsp), %rax
	jmp	.L26
	.p2align 4,,10
	.p2align 3
.L99:
	testq	%rdx, %rdx
	je	.L28
	cmpq	$1, %rdx
	je	.L101
	call	memcpy@PLT
	movq	88(%rsp), %rdx
	movq	48(%rsp), %rdi
	.p2align 4,,10
	.p2align 3
.L28:
	movq	%rdx, 56(%rsp)
	movq	(%rsp), %rax
	movb	$0, (%rdi,%rdx)
	movq	80(%rsp), %rdi
	jmp	.L30
	.p2align 4,,10
	.p2align 3
.L91:
	testq	%rdx, %rdx
	je	.L15
	cmpq	$1, %rdx
	je	.L102
	call	memcpy@PLT
	movq	120(%rsp), %rdx
	movq	48(%rsp), %rdi
	.p2align 4,,10
	.p2align 3
.L15:
	movq	%rdx, 56(%rsp)
	movb	$0, (%rdi,%rdx)
	movq	112(%rsp), %rdi
	jmp	.L17
	.p2align 4,,10
	.p2align 3
.L100:
	movq	%rsi, 48(%rsp)
	movq	%rdx, 56(%rsp)
	movq	%rax, 64(%rsp)
.L32:
	movq	(%rsp), %rax
	leaq	16(%rax), %rdi
	movq	%rdi, 80(%rsp)
	jmp	.L30
	.p2align 4,,10
	.p2align 3
.L92:
	movq	%rsi, 48(%rsp)
	movq	%rdx, 56(%rsp)
	movq	%rax, 64(%rsp)
.L19:
	leaq	16(%r13), %rdi
	movq	%rdi, 112(%rsp)
	jmp	.L17
	.p2align 4,,10
	.p2align 3
.L25:
	movq	(%rsp), %rax
	addq	$16, %rax
	testq	%rbx, %rbx
	je	.L26
	jmp	.L24
	.p2align 4,,10
	.p2align 3
.L12:
	testq	%rax, %rax
	je	.L13
	movq	%rdx, %rax
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L101:
	movzbl	96(%rsp), %eax
	movb	%al, (%rdi)
	movq	88(%rsp), %rdx
	movq	48(%rsp), %rdi
	jmp	.L28
	.p2align 4,,10
	.p2align 3
.L102:
	movzbl	128(%rsp), %eax
	movb	%al, (%rdi)
	movq	120(%rsp), %rdx
	movq	48(%rsp), %rdi
	jmp	.L15
.L93:
	leaq	.LC2(%rip), %rdi
.LEHB9:
	call	_ZSt24__throw_invalid_argumentPKc@PLT
.LEHE9:
.L95:
.LEHB10:
	call	_ZSt16__throw_bad_castv@PLT
.LEHE10:
.L94:
	leaq	.LC2(%rip), %rdi
.LEHB11:
	call	_ZSt20__throw_out_of_rangePKc@PLT
.LEHE11:
.L62:
	cmpl	$0, (%rbx)
	jne	.L86
	movl	%r13d, (%rbx)
.L86:
	movq	8(%rsp), %rdx
	movq	48(%rsp), %rdi
	movq	%rax, %rbx
	addq	$16, %rdx
	cmpq	%rdx, %rdi
	je	.L36
.L87:
	call	_ZdlPv@PLT
.L36:
	movq	%rbp, %rdi
	call	_ZNSt14basic_ifstreamIcSt11char_traitsIcEED1Ev@PLT
.L53:
	movq	%r15, %rdi
	call	_ZNSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEED1Ev@PLT
	movq	%rbx, %rdi
.LEHB12:
	call	_Unwind_Resume@PLT
.LEHE12:
.L60:
	jmp	.L86
.L58:
	movq	%rax, %rbx
	jmp	.L53
.L61:
	movq	%rax, %rbx
	movq	8(%rsp), %rax
	movq	48(%rsp), %rdi
	addq	$16, %rax
	cmpq	%rax, %rdi
	jne	.L87
	jmp	.L36
.L89:
	leaq	.LC1(%rip), %rdi
.LEHB13:
	call	_ZSt19__throw_logic_errorPKc@PLT
.L59:
	movq	%rax, %rbx
	jmp	.L36
.L97:
	leaq	.LC1(%rip), %rdi
	call	_ZSt19__throw_logic_errorPKc@PLT
.LEHE13:
	.cfi_endproc
.LFE1778:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA1778:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE1778-.LLSDACSB1778
.LLSDACSB1778:
	.uleb128 .LEHB0-.LFB1778
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB1778
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L58-.LFB1778
	.uleb128 0
	.uleb128 .LEHB2-.LFB1778
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L59-.LFB1778
	.uleb128 0
	.uleb128 .LEHB3-.LFB1778
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L60-.LFB1778
	.uleb128 0
	.uleb128 .LEHB4-.LFB1778
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L59-.LFB1778
	.uleb128 0
	.uleb128 .LEHB5-.LFB1778
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L60-.LFB1778
	.uleb128 0
	.uleb128 .LEHB6-.LFB1778
	.uleb128 .LEHE6-.LEHB6
	.uleb128 .L59-.LFB1778
	.uleb128 0
	.uleb128 .LEHB7-.LFB1778
	.uleb128 .LEHE7-.LEHB7
	.uleb128 .L60-.LFB1778
	.uleb128 0
	.uleb128 .LEHB8-.LFB1778
	.uleb128 .LEHE8-.LEHB8
	.uleb128 .L61-.LFB1778
	.uleb128 0
	.uleb128 .LEHB9-.LFB1778
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L62-.LFB1778
	.uleb128 0
	.uleb128 .LEHB10-.LFB1778
	.uleb128 .LEHE10-.LEHB10
	.uleb128 .L60-.LFB1778
	.uleb128 0
	.uleb128 .LEHB11-.LFB1778
	.uleb128 .LEHE11-.LEHB11
	.uleb128 .L62-.LFB1778
	.uleb128 0
	.uleb128 .LEHB12-.LFB1778
	.uleb128 .LEHE12-.LEHB12
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB13-.LFB1778
	.uleb128 .LEHE13-.LEHB13
	.uleb128 .L61-.LFB1778
	.uleb128 0
.LLSDACSE1778:
	.section	.text.startup
	.size	main, .-main
	.p2align 4,,15
	.type	_GLOBAL__sub_I_main, @function
_GLOBAL__sub_I_main:
.LFB2372:
	.cfi_startproc
	leaq	_ZStL8__ioinit(%rip), %rdi
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	_ZNSt8ios_base4InitC1Ev@PLT
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	leaq	__dso_handle(%rip), %rdx
	leaq	_ZStL8__ioinit(%rip), %rsi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	__cxa_atexit@PLT
	.cfi_endproc
.LFE2372:
	.size	_GLOBAL__sub_I_main, .-_GLOBAL__sub_I_main
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_main
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
