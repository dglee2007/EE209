##Donggeon Lee, Assignment#4, mydc.s

	.equ   ARRAYSIZE, 20
	.equ   EOF, -1
	
	.section ".rodata"

scanfFormat:
	.asciz "%s"
	
errorFormat:
	.asciz "dc: stack empty\n"
	
printfFormat:
	.asciz	"%d\n"
	
### ---------------------------------------------------------------

        .section ".data"
## Has the amount of numbers in stack
ele_cnt:
	.word 0

### ---------------------------------------------------------------

        .section ".bss"
buffer:
        .skip  ARRAYSIZE


### ---------------------------------------------------------------

	.section ".text"

	## --------------------------------------------------------
	## int main(void)
	## Runs desk calculator program.  Returns 0.
	## --------------------------------------------------------
	.globl  main
	.type   main,@function

main:

	pushl   %ebp
	movl    %esp, %ebp
	
	
	## 'input' gets input from stdin
input:

	## dc number stack initialized. %esp = %ebp
	
	## scanf("%s", buffer)
	pushl	$buffer
	pushl	$scanfFormat
	call    scanf
	addl    $8, %esp

	## check if user input EOF
	cmp	$EOF, %eax
	je	quit

	# increment ele_cnt
	call incr_cnt
	movl	$0, %eax
	movl	$0, %ecx

	## moves first value of buffer to '%cl'
	movb	buffer(,%eax,1), %cl
	pushl	%ecx

	## checks if first value of buffer is a number
	call	isdigit
	jne	Todigit

	## if buffer isn't number, decreases ele_cnt
	call dec_cnt

	## checks if first value of buffer is a letter
	call	isalpha
	jne	operation

	## if buffer[0] is neither number or letter,
	## it's an arthimetic operation or wrong input
	jmp	arthimetic


	## 'ChangeS' changes '_' to '-' sign
ChangeS:
	call incr_cnt
	XOR %eax, %eax
	movb	$'-', buffer(,%eax,1)
	Todigit:
		addl	$4, %esp
		pushl	$buffer
		call	atoi
		addl	$4, %esp
		pushl	%eax
		jmp	input
	
	## List of operations
	## read by buffer[0] and goes back to input
operation:
	XOR %esi, %esi
	cmpb	$'q', %cl
	je	quit
	
	cmpb	$'p', %cl
	je	oper_p
	
	cmpb	$'c', %cl
	je	oper_c
	
	cmpb	$'d', %cl
	je 	oper_d
	
	cmpb	$'r', %cl
	je	oper_r
	add	$4, %esp
	
	cmpb	$'f', %cl
	je	oper_f
	jmp	input

	## 'oper_p' is executed by 'p'
	## it prints the top stack
oper_p:
	cmpl	$1, (ele_cnt)
	jl	stackerror
	addl	$4, %esp
	pushl	$printfFormat
	call	printf
	addl	$4, %esp
	jmp	input

	## 'oper_f' is executed by 'f'
	## it prints all remaing stacks
oper_f:
	cmp	(ele_cnt), %esi
	jl	printloop
	jmp	input
	## compares # of loop excecution and
	## variable 'number'. If # of loop
	## excecution becomes 'number',
	## it goes back to 'input'
	printloop:
		movl	(%esp, %esi, 4), %ecx
		pushl	%ecx
		pushl	$printfFormat
		call	printf
		addl	$8, %esp
		incl	%esi
		jmp	oper_f

	## 'oper_c' is excecuted by 'c'
	## it clears all reamaing stacks
oper_c:
	addl	$4, %esp
	cmp	(ele_cnt), %esi	
	jne	clearloop
	jmp	input
	## compares number of loop excecution and ele_cnt. 
	## When it reachs ele_cnt, it goes back to input
	clearloop:
		call dec_cnt
		jmp	oper_c

	## 'oper_d' is excecuted by 'd'
	## it duplicates the top stack and pushes to stack.
oper_d:
	cmpl	$1, (ele_cnt)
	jl	stackerror
	movl	4(%esp), %ecx
	addl	$4, %esp
	pushl	%ecx
	call incr_cnt
	jmp	input

	## 'oper_r' is excecuted by 'r'
	## it changes order of two top stacks
oper_r:
	cmpl	$2, (ele_cnt)
	jl	stackerror
	movl	4(%esp), %ecx
	movl	8(%esp), %edx
	addl	$12, %esp
	pushl	%ecx
	pushl	%edx
	jmp	input

	## 'quit' is executed by 'q'
	## it returns 0 and quits
quit:	
	movl    $0, %eax
	movl    %ebp, %esp
	popl    %ebp
	ret

	## 'arthimetic' execute proper arthimetic operations and goes back to input
arthimetic:
	cmpb	$'_', %cl
	je	ChangeS
	cmpb	$'+', %cl
	je	oper_add
	cmpb	$'-', %cl
	je	oper_sub
	cmpb	$'*', %cl
	je	oper_mul
	cmpb	$'/', %cl
	je	oper_divd
	cmpb	$'|', %cl
	call	abssum
	addl	$4, %esp
	cmpb	$'|', %cl
	je	abssum_ps
	jmp	input

	## 'oper_add' is excecuted by '+'
	## it adds and pops two top stacks
	## pushes the result into stack
oper_add:
	cmpl	$2, (ele_cnt)
	jl	stackerror
	movl	8(%esp), %edx
	addl	4(%esp), %edx
	addl	$12, %esp
	pushl	%edx
	call dec_cnt
	jmp	input

	## 'oper_sub' is excecuted by '-'
	## it pops and substracts two top stacks
	## pushes the result into stack.
oper_sub:
	cmpl	$2, (ele_cnt)
	jl	stackerror
	movl	8(%esp), %edx
	subl	4(%esp), %edx
	addl	$12, %esp
	pushl	%edx
	call dec_cnt
	jmp	input

	## 'oper_mul' is excecuted by '*'
	## it pops and substracts two top stacks
	## pushes the result into stack.
oper_mul:
	cmpl	$2, (ele_cnt)
	jl	stackerror
	movl	8(%esp), %edx
	imul	4(%esp), %edx
	addl	$12, %esp
	pushl	%edx
	call dec_cnt
	jmp	input

	## 'oper_divd' is excecuted by '/'
	## it pops and divides two top stacks
	## pushes the result into stack.
oper_divd:
	cmpl	$2, (ele_cnt)
	jl	stackerror
	cmp	$0, 8(%esp)
	jl	mdivd
	XOR %edx, %edx
	movl	8(%esp), %eax
	movl	4(%esp), %ebx
	idivl	%ebx
	addl	$12, %esp
	pushl	%eax
	call dec_cnt
	jmp	input

	## 'mdivid' is executed from oper_divd when dividend is negative
mdivd:
	movl	$-1, %edx
	movl	8(%esp), %eax
	movl	4(%esp), %ebx
	idivl	%ebx
	addl	$12, %esp
	pushl	%eax
	call dec_cnt
	jmp	input

	## 'abssum' is called by '|'
	## it gets two input from top stacks(a, b) 
	## returns sum of absolute values from min(a,b) to max(a,b)
abssum: 
	pushl   %ebp
	movl    %esp, %ebp
	pushl	%ebx
	pushl	%esi
	call dec_cnt
	movl	24(%esp), %ebx
	movl	20(%esp), %esi
	subl	$4, %esp
	XOR -4(%ebp), -4(%ebp)
	cmpl	%ebx, %esi
	jge	pluscont
	movl	%ebx, %edi
	movl	%esi, %ebx
	movl	%edi, %esi
	## 'pluscount' adds integers from min(a,b) to max(a,b)
	pluscont:
		cmp	$0, %ebx
		jl	minuscount
		addl	%ebx, -4(%ebp)
		incl	%ebx
		cmp	%ebx, %esi
		jge	pluscont
		jmp	endsum
	## 'minuscount' is executed when adding integer is negative
	minuscount:
		subl	%ebx, -4(%ebp)
		incl	%ebx
		cmp	%ebx, %esi
		jge	pluscont
		jmp	endsum
	## 'endsum' moves return value to '%eax' and pops '%ebp'
	endsum:
		movl	-4(%ebp), %eax
		movl	%ebp, %esp
		popl	%ebp
		ret

abssum_ps:
	addl	$8, %esp
	pushl	%eax
	jmp	input

incr_cnt:
	movl (ele_cnt), %edx
	inc %edx
	movl %edx, (ele_cnt)
	ret

dec_cnt:
	movl (ele_cnt), %edx
	dec %edx
	movl %edx, (ele_cnt)
	ret

	## 'stackerror' is excecuted  when items in stacks is not enough.
stackerror:
	addl	$4, %esp
	pushl	$errorFormat
	call	printf
	addl	$4, %esp
	jmp	input
