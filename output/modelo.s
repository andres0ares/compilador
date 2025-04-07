
        .section .text
        .globl _start

        _start:
        
        mov $45, %rax
        push %rax
        mov $3, %rax 
        pop %rbx
         
            sub %rax, %rbx
            mov %rbx, %rax
             

        call imprime_num
        call sair

        .include "./output/runtime.s"
    