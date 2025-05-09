#include "declaracao.h"
#include <iostream>
#include <string>
#include <map>
#include <cctype>

using namespace std;

// Removedor de espaços
std::string trimLeft2(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : str.substr(start);
}

// Construtor
Declaracao::Declaracao(Token t, Expressao* e) : 
    token(t),
    expressao(e)
{}

Token Declaracao::getToken() const {
    return token;
}

std::string Declaracao::getTokenLexema() const {
    return token.getValue();
}

Expressao* Declaracao::getExpressao() const {
    return expressao;
}

std::set<std::string> Declaracao::getVariaveis() const {
    std::set<std::string> vars; // = expressao->getVariaveis();  // variáveis usadas na expressão
    vars.insert(token.getValue());                           // variável que está sendo atribuída
    return vars;
}

std::string Declaracao::gerar_codigo() const {
    return "\t" + trimLeft2(expressao->gerar_codigo()) + "\tmov %rax, " + token.getValue();
}

Tipo Declaracao::getTipo() const {
    return token.getTipo();
}

void Declaracao::printToken() const {
    return token.printToken();
}


