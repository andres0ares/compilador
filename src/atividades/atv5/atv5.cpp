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
        throw std::invalid_argument("\nExcesso de Parentese de abertura ou Expressão Vazia!");
    }
    
    if (token->getTipo() == Tipo::NUMERO) {
        return new Constante(std::stoi(token->getValue()));
    }
    else if (token->getTipo() == Tipo::PAREN_ESQ) {
        Expressao* esquerda = construirArvore(tokens);
        std::optional<Token> operador = tokens.proximoToken();

        //quando se espera um operador, mas se recebe parentese
        if(operador->getTipo() == Tipo::PAREN_ESQ || operador->getTipo() == Tipo::PAREN_DIR) {
            operador->printToken();
            throw std::invalid_argument("\nToken mal posicionado");
        }

        Expressao* direita = construirArvore(tokens);
        auto paren_direito = tokens.proximoToken(); //consome parentese direito

        //quando a operacao binaria nao tem parentese de fechamento
        if(paren_direito->getTipo() != Tipo::PAREN_DIR) {
            paren_direito->printToken();

            throw std::invalid_argument("\nAusencia do Parentese direito");
        }
        return new OperacaoBin(operador->getValue()[0], esquerda, direita);
    }
    //erros gerais
    throw std::invalid_argument("\nExpressão malformada");
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
        std::cout << "\nArvore Sintatica:\n" << endl;
        raiz->imprimir();
        std::cout << "\nValor do programa: " << raiz->avaliar() << std::endl;

    
        } catch (const std::invalid_argument& e) {
            std::cerr << "Erro: " << e.what() << std::endl;
            return 1;
        }
    return 0;
}
