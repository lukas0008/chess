.text
.global _generator_destroy
_generator_destroy:
    pop %rdi
    mov 16(%rdi), %rsp
    mov 80(%rdi), %rbx
    mov 88(%rdi), %r12
    mov 96(%rdi), %r13
    mov 104(%rdi), %r14
    mov 112(%rdi), %r15
    pop %rbp
    jmp generator_destroy

    # mov 32(%rdi), %rsi
    # movq $0, (%rsi)
    # mov 16(%rdi), %rsp
    # mov $STACK_SIZE, %rsi
    # call munmap
    # ret

.global _generator_next
_generator_next:
    mov (%rdi), %rdi
    mov %rbx, 80(%rdi)
    mov %r12, 88(%rdi)
    mov %r13, 96(%rdi)
    mov %r14, 104(%rdi)
    mov %r15, 112(%rdi)
    # mov %rdi, 32(%rsi)
    # mov %rsi, %rdi
    # pop %rax
    # mov %rax, 40(%rdi)
    # mov %rsp, 8(%rdi)
    # mov %rbp, 16(%rdi)
    # mov 24(%rdi), %rsp
    # mov 32(%rdi), %rsp
    # mov 48(%rdi), %rsi
    push %rbp
    mov %rsp, 16(%rdi)
    # movq $generator_next.ret, 24(%rdi)
    mov (%rdi), %rsi
    mov 8(%rdi), %rsp
    mov 40(%rdi), %rbx
    mov 48(%rdi), %r12
    mov 56(%rdi), %r13
    mov 64(%rdi), %r14
    mov 72(%rdi), %r15
    mov 24(%rdi), %rdi
    # xchg %rsi, %rdi
    jmp *%rsi

generator_next.nth:
    pop %rbp
    ret


.global _generator_pass_value
_generator_pass_value:
    mov %rbx, 40(%rdi)
    mov %r12, 48(%rdi)
    mov %r13, 56(%rdi)
    mov %r14, 64(%rdi)
    mov %r15, 72(%rdi)
    # pop %rax
    # movq %rax, (%rdi)
    push %rbp
    # mov 24(%rdi), %rax
    mov %rsp, 8(%rdi)
    mov 16(%rdi), %rsp
    lea generator_next.nth(%rip), %rax
    mov %rax, (%rdi)
    # jmp *%rax
    mov 80(%rdi), %rbx
    mov 88(%rdi), %r12
    mov 96(%rdi), %r13
    mov 104(%rdi), %r14
    mov 112(%rdi), %r15
    mov %rsi, %rax

# generator_next.ret:
    pop %rbp
    ret
    # pop %rax # return address
    # mov %rax, (%rdi)
    # mov %rsp, 24(%rdi)
    # mov %rbp, 32(%rdi)
    # mov 8(%rdi), %rsp
    # mov 16(%rdi), %rbp
    # mov %rsi, %rax
    # mov 40(%rdi), %rdi
    # jmp *%rdi
