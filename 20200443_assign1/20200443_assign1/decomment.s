	.file	"decomment.c"
	.globl	flag_r
	.bss
	.align 4
	.type	flag_r, @object
	.size	flag_r, 4
flag_r:
	.zero	4
	.globl	flag_q
	.align 4
	.type	flag_q, @object
	.size	flag_q, 4
flag_q:
	.zero	4
	.globl	flag_a
	.align 4
	.type	flag_a, @object
	.size	flag_a, 4
flag_a:
	.zero	4
	.globl	line
	.align 4
	.type	line, @object
	.size	line, 4
line:
	.zero	4
	.globl	line_r
	.align 4
	.type	line_r, @object
	.size	line_r, 4
line_r:
	.zero	4
	.comm	c,4,4
	.text
	.globl	doubleslash
	.type	doubleslash, @function
doubleslash:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	call	getchar@PLT
	movl	%eax, c(%rip)
	movl	c(%rip), %eax
	cmpl	$47, %eax
	jne	.L2
	movl	$0, %eax
	call	doubleslash
	jmp	.L6
.L2:
	movl	c(%rip), %eax
	cmpl	$42, %eax
	jne	.L4
	movl	$1, flag_r(%rip)
	movl	$32, %edi
	call	putchar@PLT
	jmp	.L6
.L4:
	movl	c(%rip), %eax
	cmpl	$10, %eax
	jne	.L5
	movl	line(%rip), %eax
	addl	$1, %eax
	movl	%eax, line(%rip)
	jmp	.L6
.L5:
	movl	$47, %edi
	call	putchar@PLT
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
.L6:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	doubleslash, .-doubleslash
	.globl	doublestar
	.type	doublestar, @function
doublestar:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	call	getchar@PLT
	movl	%eax, c(%rip)
	movl	c(%rip), %eax
	cmpl	$42, %eax
	jne	.L8
	movl	$0, %eax
	call	doublestar
	jmp	.L11
.L8:
	movl	c(%rip), %eax
	cmpl	$10, %eax
	jne	.L10
	movl	line(%rip), %eax
	addl	$1, %eax
	movl	%eax, line(%rip)
	jmp	.L11
.L10:
	movl	c(%rip), %eax
	cmpl	$47, %eax
	jne	.L11
	movl	$0, flag_r(%rip)
.L11:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	doublestar, .-doublestar
	.section	.rodata
	.align 8
.LC0:
	.string	"Error: line %d: unterminated comment\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	call	getchar@PLT
	movl	%eax, c(%rip)
	movl	c(%rip), %eax
	cmpl	$-1, %eax
	je	.L14
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
	movl	$1, line(%rip)
	jmp	.L14
.L36:
	movl	flag_r(%rip), %eax
	testl	%eax, %eax
	jne	.L15
	movl	flag_q(%rip), %eax
	testl	%eax, %eax
	jne	.L16
	movl	flag_a(%rip), %eax
	testl	%eax, %eax
	jne	.L16
	movl	c(%rip), %eax
	cmpl	$47, %eax
	jne	.L17
	call	getchar@PLT
	movl	%eax, c(%rip)
	movl	c(%rip), %eax
	cmpl	$42, %eax
	jne	.L18
	movl	$1, flag_r(%rip)
	movl	$32, %edi
	call	putchar@PLT
	movl	line(%rip), %eax
	movl	%eax, line_r(%rip)
	jmp	.L14
.L18:
	movl	c(%rip), %eax
	cmpl	$47, %eax
	jne	.L20
	movl	$47, %edi
	call	putchar@PLT
	movl	$0, %eax
	call	doubleslash
	jmp	.L14
.L20:
	movl	$47, %edi
	call	putchar@PLT
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
	jmp	.L14
.L17:
	movl	c(%rip), %eax
	cmpl	$10, %eax
	jne	.L22
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
	movl	line(%rip), %eax
	addl	$1, %eax
	movl	%eax, line(%rip)
	jmp	.L14
.L22:
	movl	c(%rip), %eax
	cmpl	$34, %eax
	jne	.L23
	movl	$1, flag_q(%rip)
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
	jmp	.L14
.L23:
	movl	c(%rip), %eax
	cmpl	$39, %eax
	jne	.L24
	movl	$1, flag_a(%rip)
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
	jmp	.L14
.L24:
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
	jmp	.L14
.L16:
	movl	flag_q(%rip), %eax
	cmpl	$1, %eax
	jne	.L26
	movl	c(%rip), %eax
	cmpl	$34, %eax
	jne	.L27
	movl	$0, flag_q(%rip)
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
	jmp	.L14
.L27:
	movl	c(%rip), %eax
	cmpl	$10, %eax
	jne	.L29
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
	movl	line(%rip), %eax
	addl	$1, %eax
	movl	%eax, line(%rip)
	jmp	.L14
.L29:
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
	jmp	.L14
.L26:
	movl	flag_a(%rip), %eax
	cmpl	$1, %eax
	jne	.L14
	movl	c(%rip), %eax
	cmpl	$39, %eax
	jne	.L30
	movl	$0, flag_a(%rip)
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
	jmp	.L14
.L30:
	movl	c(%rip), %eax
	cmpl	$10, %eax
	jne	.L31
	movl	$10, %edi
	call	putchar@PLT
	movl	line(%rip), %eax
	addl	$1, %eax
	movl	%eax, line(%rip)
	jmp	.L14
.L31:
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
	jmp	.L14
.L15:
	movl	flag_r(%rip), %eax
	cmpl	$1, %eax
	jne	.L14
	movl	c(%rip), %eax
	cmpl	$42, %eax
	jne	.L32
	call	getchar@PLT
	movl	%eax, c(%rip)
	movl	c(%rip), %eax
	cmpl	$47, %eax
	jne	.L33
	movl	$0, flag_r(%rip)
	jmp	.L14
.L33:
	movl	c(%rip), %eax
	cmpl	$42, %eax
	jne	.L35
	movl	$0, %eax
	call	doublestar
	jmp	.L14
.L35:
	movl	c(%rip), %eax
	cmpl	$10, %eax
	jne	.L14
	movl	$10, %edi
	call	putchar@PLT
	jmp	.L14
.L32:
	movl	c(%rip), %eax
	cmpl	$10, %eax
	jne	.L14
	movl	line(%rip), %eax
	addl	$1, %eax
	movl	%eax, line(%rip)
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	putchar@PLT
.L14:
	call	getchar@PLT
	movl	%eax, c(%rip)
	movl	c(%rip), %eax
	cmpl	$-1, %eax
	jne	.L36
	movl	flag_r(%rip), %eax
	cmpl	$1, %eax
	jne	.L37
	movl	line_r(%rip), %edx
	movq	stderr(%rip), %rax
	leaq	.LC0(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %eax
	jmp	.L38
.L37:
	movl	flag_r(%rip), %eax
	testl	%eax, %eax
	jne	.L39
	movl	$0, %eax
	jmp	.L38
.L39:
	movl	$0, %eax
.L38:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.5.0-1ubuntu2) 5.4.1 20171010"
	.section	.note.GNU-stack,"",@progbits
