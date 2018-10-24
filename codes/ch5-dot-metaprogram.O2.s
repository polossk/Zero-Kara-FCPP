	.file	"ch5-dot-metaprogram.cpp"
	.text
	.section	.text$_ZNKSt5ctypeIcE8do_widenEc,"x"
	.linkonce discard
	.align 2
	.p2align 4,,15
	.globl	_ZNKSt5ctypeIcE8do_widenEc
	.def	_ZNKSt5ctypeIcE8do_widenEc;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZNKSt5ctypeIcE8do_widenEc
_ZNKSt5ctypeIcE8do_widenEc:
.LFB1351:
	.seh_endprologue
	movl	%edx, %eax
	ret
	.seh_endproc
	.text
	.p2align 4,,15
	.def	__tcf_2;	.scl	3;	.type	32;	.endef
	.seh_proc	__tcf_2
__tcf_2:
.LFB3444:
	.seh_endprologue
	movq	b(%rip), %rcx
	testq	%rcx, %rcx
	je	.L3
	jmp	_ZdlPv
	.p2align 4,,10
.L3:
	ret
	.seh_endproc
	.p2align 4,,15
	.def	__tcf_1;	.scl	3;	.type	32;	.endef
	.seh_proc	__tcf_1
__tcf_1:
.LFB3443:
	.seh_endprologue
	movq	a(%rip), %rcx
	testq	%rcx, %rcx
	je	.L5
	jmp	_ZdlPv
	.p2align 4,,10
.L5:
	ret
	.seh_endproc
	.p2align 4,,15
	.def	__tcf_0;	.scl	3;	.type	32;	.endef
	.seh_proc	__tcf_0
__tcf_0:
.LFB3442:
	.seh_endprologue
	leaq	_ZStL8__ioinit(%rip), %rcx
	jmp	_ZNSt8ios_base4InitD1Ev
	.seh_endproc
	.section	.text.startup,"x"
	.align 2
	.p2align 4,,15
	.def	_ZNSt6vectorIiSaIiEEC1ESt16initializer_listIiERKS0_.isra.34.constprop.36;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZNSt6vectorIiSaIiEEC1ESt16initializer_listIiERKS0_.isra.34.constprop.36
_ZNSt6vectorIiSaIiEEC1ESt16initializer_listIiERKS0_.isra.34.constprop.36:
.LFB3481:
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$40, %rsp
	.seh_stackalloc	40
	.seh_endprologue
	movq	(%rdx), %rsi
	movq	%rcx, %rbx
	movq	$0, (%rcx)
	movq	$0, 8(%rcx)
	movq	$0, 16(%rcx)
	movl	$16, %ecx
.LEHB0:
	call	_Znwy
.LEHE0:
	leaq	16(%rax), %rdx
	movq	%rax, (%rbx)
	movq	%rdx, 16(%rbx)
	movdqu	(%rsi), %xmm0
	movq	%rdx, 8(%rbx)
	movups	%xmm0, (%rax)
	addq	$40, %rsp
	popq	%rbx
	popq	%rsi
	ret
.L11:
	movq	(%rbx), %rcx
	movq	%rax, %rsi
	testq	%rcx, %rcx
	je	.L10
	call	_ZdlPv
.L10:
	movq	%rsi, %rcx
.LEHB1:
	call	_Unwind_Resume
	nop
.LEHE1:
	.def	__gxx_personality_seh0;	.scl	2;	.type	32;	.endef
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDA3481:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE3481-.LLSDACSB3481
.LLSDACSB3481:
	.uleb128 .LEHB0-.LFB3481
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L11-.LFB3481
	.uleb128 0
	.uleb128 .LEHB1-.LFB3481
	.uleb128 .LEHE1-.LEHB1
	.uleb128 0
	.uleb128 0
.LLSDACSE3481:
	.section	.text.startup,"x"
	.seh_endproc
	.section .rdata,"dr"
.LC0:
	.ascii "======== \0"
.LC1:
	.ascii " ========\12\0"
.LC2:
	.ascii "dot():      \0"
.LC4:
	.ascii "ms\12\0"
.LC5:
	.ascii "dot_re():   \0"
.LC6:
	.ascii "dot_meta(): \0"
	.align 8
.LC7:
	.ascii "(sum1 % 100, sum2 % 100, sum3 % 100)= (\0"
.LC8:
	.ascii ", \0"
.LC9:
	.ascii ")\12\0"
	.text
	.p2align 4,,15
	.globl	_Z9benchmarki
	.def	_Z9benchmarki;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z9benchmarki
_Z9benchmarki:
.LFB2830:
	pushq	%r15
	.seh_pushreg	%r15
	pushq	%r14
	.seh_pushreg	%r14
	pushq	%r13
	.seh_pushreg	%r13
	pushq	%r12
	.seh_pushreg	%r12
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rdi
	.seh_pushreg	%rdi
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$72, %rsp
	.seh_stackalloc	72
	movaps	%xmm6, 48(%rsp)
	.seh_savexmm	%xmm6, 48
	.seh_endprologue
	movl	%ecx, %ebx
	call	_ZNSt6chrono3_V212system_clock3nowEv
	testl	%ebx, %ebx
	movq	%rax, 40(%rsp)
	jle	.L17
	movq	a(%rip), %r8
	xorl	%r11d, %r11d
	xorl	%edi, %edi
	movq	b(%rip), %r10
	leaq	16(%r8), %r9
	.p2align 4,,10
.L22:
	xorl	%ebp, %ebp
	.p2align 4,,10
.L20:
	xorl	%eax, %eax
	xorl	%ecx, %ecx
	.p2align 4,,10
.L18:
	movl	(%r10,%rax), %edx
	imull	(%r8,%rax), %edx
	addq	$4, %rax
	addl	%edx, %ecx
	cmpq	$16, %rax
	jne	.L18
	movq	%r8, %rdx
	xorl	%esi, %esi
	.p2align 4,,10
.L19:
	movl	(%rdx), %eax
	addq	$4, %rdx
	imull	%eax, %eax
	addl	%eax, %esi
	cmpq	%r9, %rdx
	jne	.L19
	imull	%esi, %ecx
	leal	1(%rbp), %eax
	addl	%ecx, %edi
	cmpl	%eax, %ebx
	je	.L41
	movl	%eax, %ebp
	jmp	.L20
	.p2align 4,,10
.L41:
	leal	1(%r11), %eax
	cmpl	%ebp, %r11d
	je	.L21
	movl	%eax, %r11d
	jmp	.L22
.L21:
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movq	a(%rip), %r8
	xorl	%r14d, %r14d
	xorl	%r12d, %r12d
	movq	b(%rip), %r9
	movq	%rax, %rsi
	movl	(%r8), %r11d
	movl	4(%r8), %eax
	movl	8(%r8), %ecx
	movl	12(%r8), %edx
	movl	12(%r9), %r13d
	imull	%r11d, %r11d
	imull	%eax, %eax
	imull	%ecx, %ecx
	imull	%edx, %r13d
	addl	%eax, %r11d
	imull	%edx, %edx
	addl	%ecx, %r11d
	addl	%edx, %r11d
	.p2align 4,,10
.L28:
	xorl	%r10d, %r10d
	.p2align 4,,10
.L25:
	xorl	%eax, %eax
	xorl	%ecx, %ecx
	.p2align 4,,10
.L23:
	movl	(%r8,%rax), %edx
	imull	(%r9,%rax), %edx
	addq	$4, %rax
	addl	%edx, %ecx
	cmpq	$12, %rax
	jne	.L23
	leal	1(%r10), %eax
	addl	%r13d, %ecx
	imull	%r11d, %ecx
	addl	%ecx, %r12d
	cmpl	%ebp, %r10d
	je	.L24
	movl	%eax, %r10d
	jmp	.L25
	.p2align 4,,10
.L24:
	leal	1(%r14), %eax
	cmpl	%r14d, %ebp
	je	.L27
	movl	%eax, %r14d
	jmp	.L28
.L27:
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movq	b(%rip), %r10
	movq	a(%rip), %r8
	movq	%rax, %r13
	movl	12(%r10), %r11d
	movl	(%r8), %edx
	movl	4(%r8), %ecx
	movl	8(%r8), %eax
	movl	12(%r8), %r9d
	movl	8(%r10), %r8d
	movl	(%r10), %r15d
	imull	%r9d, %r11d
	imull	%eax, %r8d
	imull	%r9d, %r9d
	imull	%eax, %eax
	addl	%r11d, %r8d
	movl	4(%r10), %r11d
	imull	%edx, %r15d
	xorl	%r10d, %r10d
	imull	%edx, %edx
	addl	%r9d, %eax
	xorl	%r9d, %r9d
	imull	%ecx, %r11d
	imull	%ecx, %ecx
	addl	%r11d, %r8d
	addl	%ecx, %eax
	addl	%r15d, %r8d
	addl	%edx, %eax
	imull	%eax, %r8d
	movl	%r8d, %r11d
	imull	%ebp, %r11d
	addl	%r8d, %r11d
	.p2align 4,,10
.L30:
	movl	%r9d, %ecx
	xorl	%eax, %eax
	jmp	.L29
	.p2align 4,,10
.L36:
	movl	%edx, %eax
.L29:
	leal	1(%rax), %edx
	addl	%r8d, %ecx
	cmpl	%ebp, %eax
	jne	.L36
	leal	1(%r10), %edx
	addl	%r11d, %r9d
	cmpl	%r10d, %eax
	je	.L42
	movl	%edx, %r10d
	jmp	.L30
.L42:
	movl	$1374389535, %r8d
	movl	%edi, %eax
	imull	%r8d
	movl	%edi, %eax
	sarl	$31, %eax
	sarl	$5, %edx
	subl	%eax, %edx
	movl	%r12d, %eax
	imull	$100, %edx, %r14d
	imull	%r8d
	movl	%r12d, %eax
	sarl	$31, %eax
	subl	%r14d, %edi
	sarl	$5, %edx
	movl	%edi, %r14d
	subl	%eax, %edx
	movl	%ecx, %eax
	imull	$100, %edx, %r15d
	imull	%r8d
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%r15d, %r12d
	sarl	$5, %edx
	movl	%r12d, %r15d
	movl	%edx, %edi
	subl	%eax, %edi
	imull	$100, %edi, %edi
	subl	%edi, %ecx
	movl	%ecx, %edi
.L34:
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movq	.refptr._ZSt4cout(%rip), %rbp
	movl	$9, %r8d
	movq	%rax, %r12
	subq	%r13, %r12
	movq	0(%rbp), %rax
	movq	%rbp, %rcx
	movq	-24(%rax), %rdx
	addq	%rbp, %rdx
	movl	24(%rdx), %eax
	movq	$2, 8(%rdx)
	andl	$-261, %eax
	orl	$4, %eax
	movl	%eax, 24(%rdx)
	leaq	.LC0(%rip), %rdx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movl	%ebx, %edx
	movq	%rbp, %rcx
	call	_ZNSolsEi
	movl	$10, %r8d
	leaq	.LC1(%rip), %rdx
	movq	%rax, %rcx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movl	$12, %r8d
	movq	%rbp, %rcx
	leaq	.LC2(%rip), %rdx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movq	%rsi, %rax
	subq	40(%rsp), %rax
	movq	%rbp, %rcx
	pxor	%xmm1, %xmm1
	movsd	.LC3(%rip), %xmm6
	cvtsi2sdq	%rax, %xmm1
	divsd	%xmm6, %xmm1
	call	_ZNSo9_M_insertIdEERSoT_
	movl	$3, %r8d
	leaq	.LC4(%rip), %rdx
	movq	%rax, %rcx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movl	$12, %r8d
	movq	%rbp, %rcx
	leaq	.LC5(%rip), %rdx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movq	%r13, %rax
	pxor	%xmm1, %xmm1
	movq	%rbp, %rcx
	subq	%rsi, %rax
	cvtsi2sdq	%rax, %xmm1
	divsd	%xmm6, %xmm1
	call	_ZNSo9_M_insertIdEERSoT_
	movl	$3, %r8d
	leaq	.LC4(%rip), %rdx
	movq	%rax, %rcx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movl	$12, %r8d
	movq	%rbp, %rcx
	leaq	.LC6(%rip), %rdx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	pxor	%xmm1, %xmm1
	movq	%rbp, %rcx
	cvtsi2sdq	%r12, %xmm1
	divsd	%xmm6, %xmm1
	call	_ZNSo9_M_insertIdEERSoT_
	movl	$3, %r8d
	leaq	.LC4(%rip), %rdx
	movq	%rax, %rcx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movl	$39, %r8d
	movq	%rbp, %rcx
	leaq	.LC7(%rip), %rdx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movl	%r14d, %edx
	movq	%rbp, %rcx
	call	_ZNSolsEi
	movl	$2, %r8d
	leaq	.LC8(%rip), %rdx
	movq	%rax, %rbx
	movq	%rax, %rcx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movq	%rbx, %rcx
	movl	%r15d, %edx
	call	_ZNSolsEi
	movl	$2, %r8d
	leaq	.LC8(%rip), %rdx
	movq	%rax, %rbx
	movq	%rax, %rcx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movq	%rbx, %rcx
	movl	%edi, %edx
	call	_ZNSolsEi
	movl	$2, %r8d
	leaq	.LC9(%rip), %rdx
	movq	%rax, %rcx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movq	0(%rbp), %rax
	movq	-24(%rax), %rax
	movq	240(%rax,%rbp), %rbx
	testq	%rbx, %rbx
	je	.L43
	cmpb	$0, 56(%rbx)
	je	.L32
	movsbl	67(%rbx), %edx
.L33:
	movq	.refptr._ZSt4cout(%rip), %rcx
	call	_ZNSo3putEc
	nop
	movaps	48(%rsp), %xmm6
	movq	%rax, %rcx
	addq	$72, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	jmp	_ZNSo5flushEv
.L32:
	movq	%rbx, %rcx
	call	_ZNKSt5ctypeIcE13_M_widen_initEv
	movq	(%rbx), %rax
	movl	$10, %edx
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rcx
	movq	48(%rax), %rax
	cmpq	%rcx, %rax
	je	.L33
	movq	%rbx, %rcx
	call	*%rax
	movsbl	%al, %edx
	jmp	.L33
.L17:
	call	_ZNSt6chrono3_V212system_clock3nowEv
	xorl	%edi, %edi
	xorl	%r15d, %r15d
	xorl	%r14d, %r14d
	movq	%rax, %rsi
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movq	%rax, %r13
	jmp	.L34
.L43:
	call	_ZSt16__throw_bad_castv
	nop
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.section	.text.startup,"x"
	.p2align 4,,15
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB2842:
	subq	$40, %rsp
	.seh_stackalloc	40
	.seh_endprologue
	call	__main
	movl	$1000, %ecx
	call	_Z9benchmarki
	movl	$3000, %ecx
	call	_Z9benchmarki
	movl	$5000, %ecx
	call	_Z9benchmarki
	movl	$7000, %ecx
	call	_Z9benchmarki
	movl	$9000, %ecx
	call	_Z9benchmarki
	xorl	%eax, %eax
	addq	$40, %rsp
	ret
	.seh_endproc
	.p2align 4,,15
	.def	_GLOBAL__sub_I_a;	.scl	3;	.type	32;	.endef
	.seh_proc	_GLOBAL__sub_I_a
_GLOBAL__sub_I_a:
.LFB3445:
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$72, %rsp
	.seh_stackalloc	72
	.seh_endprologue
	leaq	_ZStL8__ioinit(%rip), %rcx
	call	_ZNSt8ios_base4InitC1Ev
	leaq	__tcf_0(%rip), %rcx
	call	atexit
	leaq	32(%rsp), %rbx
	movabsq	$8589934593, %rax
	movq	$4, 40(%rsp)
	movq	%rax, 48(%rsp)
	leaq	48(%rsp), %rsi
	movq	%rbx, %rdx
	movabsq	$17179869187, %rax
	leaq	a(%rip), %rcx
	movq	%rax, 56(%rsp)
	movq	%rsi, 32(%rsp)
	call	_ZNSt6vectorIiSaIiEEC1ESt16initializer_listIiERKS0_.isra.34.constprop.36
	leaq	__tcf_1(%rip), %rcx
	call	atexit
	movq	%rbx, %rdx
	movq	%rsi, 32(%rsp)
	movabsq	$25769803781, %rax
	movq	%rax, 48(%rsp)
	leaq	b(%rip), %rcx
	movabsq	$34359738375, %rax
	movq	%rax, 56(%rsp)
	movq	$4, 40(%rsp)
	call	_ZNSt6vectorIiSaIiEEC1ESt16initializer_listIiERKS0_.isra.34.constprop.36
	leaq	__tcf_2(%rip), %rcx
	call	atexit
	nop
	addq	$72, %rsp
	popq	%rbx
	popq	%rsi
	ret
	.seh_endproc
	.section	.ctors,"w"
	.align 8
	.quad	_GLOBAL__sub_I_a
	.globl	b
	.bss
	.align 16
b:
	.space 24
	.globl	a
	.align 16
a:
	.space 24
.lcomm _ZStL8__ioinit,1,1
	.section .rdata,"dr"
	.align 8
.LC3:
	.long	0
	.long	1093567616
	.ident	"GCC: (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0"
	.def	_ZdlPv;	.scl	2;	.type	32;	.endef
	.def	_ZNSt8ios_base4InitD1Ev;	.scl	2;	.type	32;	.endef
	.def	_Znwy;	.scl	2;	.type	32;	.endef
	.def	_Unwind_Resume;	.scl	2;	.type	32;	.endef
	.def	_ZNSt6chrono3_V212system_clock3nowEv;	.scl	2;	.type	32;	.endef
	.def	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x;	.scl	2;	.type	32;	.endef
	.def	_ZNSolsEi;	.scl	2;	.type	32;	.endef
	.def	_ZNSo9_M_insertIdEERSoT_;	.scl	2;	.type	32;	.endef
	.def	_ZNSo3putEc;	.scl	2;	.type	32;	.endef
	.def	_ZNSo5flushEv;	.scl	2;	.type	32;	.endef
	.def	_ZNKSt5ctypeIcE13_M_widen_initEv;	.scl	2;	.type	32;	.endef
	.def	_ZSt16__throw_bad_castv;	.scl	2;	.type	32;	.endef
	.def	_ZNSt8ios_base4InitC1Ev;	.scl	2;	.type	32;	.endef
	.def	atexit;	.scl	2;	.type	32;	.endef
	.section	.rdata$.refptr._ZSt4cout, "dr"
	.globl	.refptr._ZSt4cout
	.linkonce	discard
.refptr._ZSt4cout:
	.quad	_ZSt4cout
