#ifndef TOKENS_H
#define TOKENS_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <optional>
#include "../token/token.h"

class Tokens {
private:
    std::vector<Token> listaTokens;
    size_t indiceAtual;

public:
    Tokens();
    void adiciona(const Token& token);
    void retrocedeToken();
    void imprimeTokens() const;
    void imprimeTokensValidos() const;
    void imprimeErros() const;
    bool temTokenInvalido() const; // retorna se tem token invalido
    std::optional<Token> proximoToken();
};


#endif // TOKEN_H