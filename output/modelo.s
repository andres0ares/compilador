
        .section .text
        .globl _start

        _start:
        mov $4, %rax
        push %rax
        mov $4, %rax 
        pop %rbx
        imul %rbx, %rax
        push %rax
        mov $2, %rax 
        pop %rbx
         
            sub %rax, %rbx
            mov %rbx, %rax
            
        push %rax
        mov $2, %rax 
        pop %rbx
         
            mov %rbx, %rcx
            mov %rax, %rbx
            mov %rcx, %rax
            idiv %rbx
            
        push %rax
        mov $1, %rax 
        pop %rbx
        add %rbx, %rax 

        call imprime_num
        call sair

        .include "./output/runtime.s"
    