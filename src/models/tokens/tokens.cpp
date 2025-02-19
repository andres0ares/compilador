#include "tokens.h"
#include "../token/token.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <map>
#include <cctype>
#include <vector>

using namespace std;

// Adiciona um token à lista
void Tokens::adiciona(const Token& token) {
    listaTokens.push_back(token);
}


bool Tokens::temTokenInvalido() const {
    for (const Token& token : listaTokens) {
        if (token.getTipo() == Tipo::INVALIDO) {
            return true;
        }
    }
    return false;
}

// Imprime todos os tokens armazenados
void Tokens::imprimeTokens() const {
    for (const auto& token : listaTokens) {
        token.printToken();
    }
}

// Imprime todos os tokens armazenados
void Tokens::imprimeTokensValidos() const {
    for (const auto& token : listaTokens) {
        if (token.getTipo() != Tipo::INVALIDO) {
             token.printToken();
        }
    }
}

void Tokens::imprimeErros() const {
    for (const auto& token : listaTokens) {
        if (token.getTipo() == Tipo::INVALIDO) {
             token.printToken();
        }
    }
}

std::optional<Token> Tokens::proximoToken() {

     if (listaTokens.empty()) {
        return std::nullopt; // Retorna um valor nulo se a lista estiver vazia
    }

    Token primeiroToken = listaTokens.front(); // Obtém o primeiro token
    listaTokens.erase(listaTokens.begin()); // Remove o primeiro token

    return primeiroToken; // Retorna o token removido
}
