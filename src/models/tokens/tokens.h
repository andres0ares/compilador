#ifndef TOKENS_H
#define TOKENS_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "../token/token.h"

class Tokens {
private:
    std::vector<Token> listaTokens;

public:
    void adiciona(const Token& token);
    void imprimeTokens() const;
    void imprimeTokensValidos() const;
    void imprimeErros() const;
    bool temTokenInvalido() const; // Novo método
};


#endif // TOKEN_H