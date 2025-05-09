#ifndef DECLARACAO_H
#define DECLARACAO_H

#include <set>
#include <map>
#include <string>
#include <iostream>
#include "../token/token.h"
#include "../Expressao/Expressao.h"


class Declaracao {
private:
    Token token;
    Expressao* expressao;

public:
    Declaracao(Token t, Expressao* e);
    Token getToken() const;
    Expressao* getExpressao() const;
    Tipo getTipo() const; // retorna tipo do token: identificador ou retorna
    std::string getTokenLexema() const; // retorno o valor de token da atribuição (string)
    std::set<std::string> getVariaveis() const; // retorno o valor de token da atribuição (set)
    std::string gerar_codigo() const;
    void printToken() const; // imprime token
};

#endif // TOKEN_H