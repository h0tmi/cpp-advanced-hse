	.global SaveContext
	.global JumpContext

// Save execution context to %rdi
// %rdi == address of struct Context
// %(rdi) == first 8 bytes of struct Context
SaveContext:
	// ctx->rip = return address;
	mov	(%rsp), %rax
	mov	%rax, (%rdi)

	// ctx->rsp = %rsp;
	mov	%rsp, 1*8(%rdi)

	// Save calle-saved registers
	mov	%rbp, 2*8(%rdi)
	mov	%rbx, 3*8(%rdi)
	mov	%r12, 4*8(%rdi)
	mov	%r13, 5*8(%rdi)
	mov	%r14, 6*8(%rdi)
	mov	%r15, 7*8(%rdi)

	// return ESaveContextResult::Saved;
	mov	$0, %rax
	ret


// Load execution context from %rdi
JumpContext:
	// Switch to stack ctx->rsp;
	mov	1*8(%rdi), %rsp

	// Load calle-saved registers
	mov	2*8(%rdi), %rbp
	mov	3*8(%rdi), %rbx
	mov	4*8(%rdi), %r12
	mov	5*8(%rdi), %r13
	mov	6*8(%rdi), %r14
	mov	7*8(%rdi), %r15

	// return address = ctx->rip;
	mov	(%rdi), %rax
	mov 	%rax, (%rsp)

	// return ESaveContextResult::Resumed;
	mov	$1, %rax
	ret
