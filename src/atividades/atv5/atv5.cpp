#include "atv5.h"
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

//constroi a arvore sintatica a partir da lista de tokens
Expressao* construirArvore(Tokens& tokens) {
    std::optional<Token> token = tokens.proximoToken();

    if (!token) {
        throw std::invalid_argument("Expressao Vazia!\n");
    }
    
    if (token->getTipo() == Tipo::NUMERO) {
        std::optional<Token> proxToken = tokens.proximoToken();

        // verifica se um numero é seguido de outro numero ou parentese aberto
        if (proxToken->getTipo() == Tipo::NUMERO) {
            proxToken->printToken();
            throw std::invalid_argument("Ausência de operador entre operandos\n");
        }
        else if (proxToken->getTipo() == Tipo::PAREN_ESQ) {
            proxToken->printToken();
            throw std::invalid_argument("Esperva-se um operador\n");
        
        } else if(!proxToken)
            return new Constante(std::stoi(token->getValue()));

        // volta para a posicao do token atual
        tokens.retrocedeToken();

        return new Constante(std::stoi(token->getValue()));
    }

    else if (token->getTipo() == Tipo::PAREN_ESQ) {
        std::optional<Token> proxToken = tokens.proximoToken();
        if (!proxToken) {
            throw std::invalid_argument("Parêntese de abertura sem conteúdo\n");
        }
        if (proxToken->getTipo() == Tipo::PAREN_DIR) {
            proxToken->printToken();
            throw std::invalid_argument("Parêntese de abertura seguido de fechamento\n");
        }
        if (proxToken->getTipo() != Tipo::NUMERO && proxToken->getTipo() != Tipo::PAREN_ESQ) {
            proxToken->printToken();
            throw std::invalid_argument("Parêntese de abertura seguido de operador\n");
        }
        tokens.retrocedeToken();
        
        Expressao* esquerda = construirArvore(tokens);

        std::optional<Token> operador = tokens.proximoToken();
        if (!operador || (operador->getTipo() != Tipo::SOMA && operador->getTipo() != Tipo::SUB && 
                          operador->getTipo() != Tipo::MULT && operador->getTipo() != Tipo::DIV)) {
            throw std::invalid_argument("Operador inválido ou ausente\n");
        }
        // avanca para saber se a operacao é valida
        std::optional<Token> proxDepoisOperador = tokens.proximoToken();
        if (!proxDepoisOperador) {
            throw std::invalid_argument("Ausência do segundo operando\n");
        }
        else if(proxDepoisOperador->getTipo() == Tipo::PAREN_DIR) {
            proxDepoisOperador->printToken();
            throw std::invalid_argument("Operador seguido de parêntese fechado\n");
        }
        tokens.retrocedeToken();

        Expressao* direita = construirArvore(tokens);
        std::optional<Token> parentesesDir = tokens.proximoToken();
        if (!parentesesDir || parentesesDir->getTipo() != Tipo::PAREN_DIR) {
            throw std::invalid_argument("Parêntese de fechamento ausente\n");
        }
        return new OperacaoBin(operador->getTipo(), esquerda, direita);
    }
    throw std::invalid_argument("Erro: Expressão malformada");
}


int Atv5::analise_sintatico(ifstream& file) {

    Atv4 atv4;
    
    //cria tokens do arquivos
    Tokens tokens = atv4.create_tokens(file);

    //se possuir erro lexico exibe o erro e retorna
    if(tokens.temTokenInvalido()) {
        std::cout << "Criacao dos tokens concluida com erro lexico: \n" << endl;
        tokens.imprimeErros();
        return 1;
    } 

    try {    
        //construcao da arvore sintatica
        Expressao* raiz = construirArvore(tokens);

        // verifica se sobra tokens apos a arvore ter sido construida
        if (tokens.proximoToken()) {
            throw std::runtime_error("Expressao tem tokens extras ou pareteses desbalanceados\n");
        }

        std::cout << "\nArvore Sintatica:\n" << endl;
        raiz->imprimir();
        std::cout << "\nValor do programa: " << raiz->avaliar() << std::endl;

    
        } catch (const std::invalid_argument& e) {
            std::cerr << "\nErro: " << e.what() << std::endl;
            return 1;
        }
    return 0;
}

std::optional<Expressao*> Atv5::get_arvore(ifstream& file) {

    Atv4 atv4;
    
    //cria tokens do arquivos
    Tokens tokens = atv4.create_tokens(file);

    //se possuir erro lexico exibe o erro e retorna
    if(tokens.temTokenInvalido()) {
        //std::cout << "Criacao dos tokens concluida com erro lexico: \n" << endl;
        tokens.imprimeErros();
        throw std::runtime_error("Erro lexico");
        return std::nullopt;
    } 

    try {    
        //construcao da arvore sintatica
        Expressao* raiz = construirArvore(tokens);

        // verifica se sobra tokens apos a arvore ter sido construida
        if (tokens.proximoToken()) {
            throw std::runtime_error("Expressao tem tokens extras ou pareteses desbalanceados\n");
        }

        return raiz;

    
    } catch (const std::invalid_argument& e) {
        std::cerr << "\nErro: " << e.what() << std::endl;
        return std::nullopt;
    }

}