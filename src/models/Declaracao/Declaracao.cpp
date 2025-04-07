#include "declaracao.h"
#include <iostream>
#include <string>
#include <map>
#include <cctype>

using namespace std;

// Construtor
Declaracao::Declaracao(Token t, Expressao* e) : 
    token(t),
    expressao(e)
{}

Token Declaracao::getToken() const {
    return token;
}

Expressao* Declaracao::getExpressao() const {
    return expressao;
}

std::string Declaracao::getIdentificador() const {
    return token.getValue();
}

std::string Declaracao::getCodigoExp() const {
    return expressao->gerar_codigo();
}

std::string Declaracao::gerar_codigo() const {
    return expressao->gerar_codigo();
}

Tipo Declaracao::getTipo() const {
    return token.getTipo();
}

void Declaracao::printToken() const {
    return token.printToken();
}


