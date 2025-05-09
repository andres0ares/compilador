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

std::set<std::string> Programa::getVariaveisDeclaradas() const {
    std::set<std::string> vars;
    for (const auto& decl : declaracoes) {
        std::set<std::string> varsDecl = decl.getVariaveis();
        vars.insert(varsDecl.begin(), varsDecl.end());  // insere todos os elementos do conjunto
    }
    return vars;
}

// Adiciona um token à lista
void Programa::adiciona(const Declaracao& declaracao) {
    declaracoes.push_back(declaracao);
}

void Programa::adicionaExpressao(Expressao* expressao) {
    expressao_retorno = expressao;
}

void Programa::imprimeTokens() const {
    for (const auto& declaracao : declaracoes) {
        declaracao.printToken();
    }
}

bool Programa::variavelDeclarada(const std::string& var) const {
    for (const auto& declaracao : declaracoes) {
        std::set<std::string> vars = declaracao.getVariaveis();
        if (vars.find(var) != vars.end()) {
            return true;
        }
    }
    return false;
}


void Programa::adicionaComando(Comando* cmd) {
    comandos.push_back(cmd);
}

const std::vector<Comando*>& Programa::getComandos() const {
    return comandos;
}

std::string Programa::gerar_declaracoes() const {

    std::string saida = "";

    if(declaracoes.size() == 0) {
        return saida;
    }

    for (const auto& declaracao : declaracoes) {
        saida += "\n        .lcomm " + declaracao.getTokenLexema() + ", 8";
    }

    return R"(
        .section .bss
        )" + saida + R"( 
    )";

}

std::string Programa::gerar_codigo() const {

    std::string saida = "\n";

    for (const auto& declaracao : declaracoes) {
        saida += declaracao.gerar_codigo();
        saida += "\n        mov %rax, " + declaracao.getTokenLexema() + "\n"; 
    }
    saida += "\n";

    for (const auto& cmd : comandos) {
        saida += cmd->gerar_codigo();
    }

    saida += expressao_retorno->gerar_codigo();

    return saida;
}