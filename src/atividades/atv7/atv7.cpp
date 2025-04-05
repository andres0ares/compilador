#include "atv7.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <cctype>

#include "../../models/token/token.h"
#include "../../models/tokens/tokens.h"
#include "../../models/Expressao/Expressao.h"
#include "../atv4/atv4.h"

using namespace std;

Expressao* Atv7::expAdv(Tokens& tokens) {
    Expressao* esquerda = expMult(tokens);

    while (true) {
        std::optional<Token> token = tokens.proximoToken();

        if (!token || (token->getTipo() != Tipo::SOMA && token->getTipo() != Tipo::SUB)) {
            if (token) 
                tokens.retrocedeToken(); // devolve se não for operador válido
            break;
        }

        Expressao* direita = expMult(tokens);
        esquerda = new OperacaoBin(token->getValue()[0], esquerda, direita);
    }
    return esquerda;
}

Expressao* Atv7::expMult(Tokens& tokens) {
    Expressao* esquerda = expPrim(tokens);

    while (true) {
        std::optional<Token> token = tokens.proximoToken();

        if (!token || (token->getTipo() != Tipo::MULT && token->getTipo() != Tipo::DIV)) {
            if (token) 
                tokens.retrocedeToken(); // devolve se não for operador válido
            break;
        }

        Expressao* direita = expPrim(tokens);
        esquerda = new OperacaoBin(token->getValue()[0], esquerda, direita);
    }
    return esquerda;
}

Expressao* Atv7::expPrim(Tokens& tokens) {
    std::optional<Token> token = tokens.proximoToken();

    if (!token) {
        throw std::invalid_argument("Erro: expressão vazia ou token inesperado no final da entrada.\n");
    }

    // Uso em operações unários, tipo: -5
    if (token->getTipo() == Tipo::SUB) {
        Expressao* termo = expPrim(tokens);
        if (!termo) 
            return nullptr;

        return new OperacaoBin(token->getValue()[0], new Constante(0), termo); // 0 - termo
    }
    
    // Identificação de número
    if (token->getTipo() == Tipo::NUMERO) {
        int valor = std::stoi(token->getValue());
        return new Constante(valor);
    }

    // Abertura para nova expressão
    if (token->getTipo() == Tipo::PAREN_ESQ) {
        Expressao* exp = expAdv(tokens);
        std::optional<Token> proxToken = tokens.proximoToken();

        // Espera pelo fechamento da expressão
        if (!proxToken || proxToken->getTipo() != Tipo::PAREN_DIR) {            
            throw std::invalid_argument("Erro: parêntese de fechamento ')' esperado.\n");
        }
        return exp;
    }

    std::cerr << "Erro: token inesperado na expressão primária:\n";
    token->printToken();
    return nullptr;
}


std::optional<Expressao*> Atv7::analise_sintatico(ifstream& file) {
    Atv4 atv4;
    Tokens tokens = atv4.create_tokens(file);

    if (tokens.temTokenInvalido()) {
        std::cout << "Criação dos tokens concluída com erro léxico:\n" << std::endl;
        tokens.imprimeErros();
        return std::nullopt;
    }

    try {
        Expressao* raiz = expAdv(tokens);

        std::optional<Token> sobra = tokens.proximoToken();
        if (sobra.has_value()) {
            throw std::invalid_argument("Erro: expressão possui tokens extras ou parênteses desbalanceados.");
            return std::nullopt;
        }
        return raiz;
        // std::cout << "\nÁrvore Sintática:\n" << std::endl;
        // raiz->imprimir();
        // std::cout << "\nValor do programa: " << raiz->avaliar() << std::endl;
    } 
    catch (const std::invalid_argument& e) {
        std::cerr << "\n- " << e.what() << std::endl;
        return std::nullopt;
    }
}

