
        .section .text
        .globl _start

        _start:
        mov $6, %rax
        push %rax
        mov $7, %rax 
        pop %rbx
        add %rbx, %rax
        push %rax
        mov $45, %rax 
        pop %rbx
        add %rbx, %rax 

        call imprime_num
        call sair

        .include "./output/runtime.s"
    