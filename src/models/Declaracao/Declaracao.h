#ifndef DECLARACAO_H
#define DECLARACAO_H

#include <iostream>
#include <map>
#include <string>
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
    std::string getIdentificador() const;
    std::string getCodigoExp() const;
    std::string gerar_codigo() const;
    void printToken() const; // imprime token
};

#endif // TOKEN_H