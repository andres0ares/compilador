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

//          script do professor         //
// void OperacaoBin::imprimir() const  {
//     std::cout << "(";
//     esquerda->imprimir();
//     std::cout << " " << operador << " ";
//     direita->imprimir();
//     std::cout << ")";
// }

//print da arvore de forma horizontal
void OperacaoBin::imprimir(int nivel = 0) const {
    direita->imprimir(nivel + 1);
    std::cout << std::string(nivel * 4, ' ') << operador << std::endl;
    esquerda->imprimir(nivel + 1);
}

// ========================================================================
