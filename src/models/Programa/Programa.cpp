#include "Programa.h"
#include "../Declaracao/Declaracao.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <map>
#include <cctype>
#include <vector>

using namespace std;

Programa::Programa() {
    indiceAtual = 0;
}

// Adiciona um token à lista
void Programa::adiciona(const Declaracao& declaracao) {
    declaracoes.push_back(declaracao);
}

void Programa::adicionaFinal(const Declaracao& declaracao) {
    dec_final = declaracao;
}

void Programa::imprimeTokens() const {
    for (const auto& declaracao : declaracoes) {
        declaracao.printToken();
    }
    dec_final->printToken();
}

bool Programa::identificadorDeclarado(std::string ident) const {
    for (const auto& declaracao : declaracoes) {
        if(declaracao.getIdentificador() == ident) {
            return true;
        }
    }
    return false;
}

std::string Programa::gerar_declaracoes() const {

    std::string saida = "";

    if(declaracoes.size() == 0) {
        return saida;
    }

    for (const auto& declaracao : declaracoes) {
        saida += "\n        .lcomm " + declaracao.getIdentificador() + ", 8";
    }

    return R"(
        .section .bss
        )" + saida + R"( 
    )";

}

std::string Programa::gerar_codigo() const {

    std::string saida = "";

    for (const auto& declaracao : declaracoes) {
        saida += "\n" + declaracao.gerar_codigo();
        saida += "\n        mov %rax, " + declaracao.getIdentificador(); 
    }

    saida += "\n" + dec_final->gerar_codigo();

    return saida;
}