#include "../token/token.h"
#include "Expressao.h"
#include <iostream>
#include <string>

Constante::Constante(int v) : 
    valor(v) 
{}

int Constante::avaliar() const {
    return valor;
}

// void Constante::imprimir() const {
//     std::cout << valor; 
// }

void Constante::imprimir(int nivel = 0) const {
    std::cout << std::string(nivel * 4, ' ') << valor << std::endl;
}

// ========================================================================

OperacaoBin::OperacaoBin(char op, Expressao* esq, Expressao* dir) :
    operador(op),
    esquerda(esq), 
    direita(dir) 
{}


int OperacaoBin::avaliar() const {
    int valorEsq = esquerda->avaliar();
    int valorDir = direita->avaliar();

    switch (operador) {
        case '+': 
            return valorEsq + valorDir;
        case '-': 
            return valorEsq - valorDir;
        case '*': 
            return valorEsq * valorDir;
        case '/': 
            return valorDir != 0 ? valorEsq / valorDir : throw std::invalid_argument("Divisao por 0");

        default: throw std::runtime_error("Operador desconhecido");
    }
}


//print da arvore de forma horizontal
void OperacaoBin::imprimir(int nivel = 0) const {
    direita->imprimir(nivel + 1);
    std::cout << std::string(nivel * 4, ' ') << operador << std::endl;
    esquerda->imprimir(nivel + 1);
}

// ============== GERAR CODIGO ============================

std::string operador_assembly(char op) {
    switch (op) {
        case '+': 
            return "add %rbx, %rax";
        case '-': 
            return R"( 
            sub %rax, %rbx
            mov %rbx, %rax
            )";
        case '*': 
            return "imul %rbx, %rax";
        case '/': 
            return R"( 
            mov %rbx, %rcx
            mov %rax, %rbx
            mov %rcx, %rax
            idiv %rbx
            )";
        default: throw std::runtime_error("Operador desconhecido");
    }
}

std::string OperacaoBin::gerar_codigo() const {

    std::string op_esq = esquerda->gerar_codigo();
    std::string op_dir = direita->gerar_codigo();

    std::string operacao = operador_assembly(operador); 

    return op_esq + R"(
        push %rax
        )" + op_dir + R"( 
        pop %rbx
        )" + operacao;
}

std::string Constante::gerar_codigo() const {
    return "mov $" + std::to_string(valor) + ", %rax";
}

// ========================================================================
