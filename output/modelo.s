
        .section .bss
        
        .lcomm a, 8 
    
        .section .text
        .globl _start

        _start:
        
	mov $1, %rax
	mov %rax, a
        mov %rax, a

		mov a, %rax

        push %rax
        mov $1, %rax
 
        pop %rbx
        add %rbx, %rax 

        call imprime_num
        call sair

        .include "./output/runtime.s"
    