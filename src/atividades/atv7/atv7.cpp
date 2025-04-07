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
#include "../../models/Declaracao/Declaracao.h"
#include "../../models/Programa/Programa.h"
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
    if (token->getTipo() == Tipo::NUMERO ) {
        int valor = std::stoi(token->getValue());
        return new Constante(valor);
    }

    //Identificacao de identificador
    if (token->getTipo() == Tipo::IDENTIFICADOR ) {
        return new Identificador(token->getValue());
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


bool verifica_identificadores(Programa prog, std::string idents) {

    std::stringstream ss(idents);
    std::string ident;

    while(ss >> ident) {
        if(!prog.identificadorDeclarado(ident)) {
            throw std::invalid_argument("Erro: Identificador nao declarado: " + ident);
        }
    }
    
    return true;
}

Programa Atv7::prog(Tokens& tokens) {

    //le primeiro token
    std::optional<Token> token = tokens.proximoToken();
    std::optional<Token> ident;
    Expressao* exp;

    //se vazio, retorna erro
    if (!token) {
        throw std::invalid_argument("Erro: expressão vazia ou token inesperado no final da entrada.\n");
    }

    //cria objeto programa
    Programa programa;

    //para cada identificado
    while(token->getTipo() == Tipo::IDENTIFICADOR) {

        ident = token; //salva token do identificador
        token = tokens.proximoToken(); //le proximo token

        //se nao for (=) retorna erro
        if (!token || token->getTipo() != Tipo::IGUAL) {
            token->printToken();
            throw std::invalid_argument("Erro: esperado '=' apos identificador.\n");
        }

        //le a expressao
        exp = expAdv(tokens);

        //le proximo token e retorna erro se nao for ;
        token = tokens.proximoToken();

        if (!token || token->getTipo() != Tipo::PTVIRG) {
            token->printToken();
            throw std::invalid_argument("Erro: esperado ;\n");
        }

        //verifica os identicadores da expressao lida
        verifica_identificadores(programa, exp->getIdentificadores());
        
        //adiciona declaracao
        Declaracao  declaracao(ident.value(), exp);
        programa.adiciona(declaracao);

        token = tokens.proximoToken();
    }

    //espera pela token da expressao final (retorna)
    if(token->getTipo() != Tipo::RETORNA ) {
        token->printToken();
        throw std::invalid_argument("Erro: esperado -> retorna.\n");
    }

    //le expressao
    exp = expAdv(tokens);

    //verifica os identicadores da expressao lida
    verifica_identificadores(programa, exp->getIdentificadores());

    //adiciona a expressao final
    Declaracao  declaracao(token.value(), exp);
    programa.adicionaFinal(declaracao);

    return programa;

}

std::optional<Programa> Atv7::analise_sintatico(ifstream& file) {
    Atv4 atv4;
    Tokens tokens = atv4.create_tokens(file);

    if (tokens.temTokenInvalido()) {
        std::cout << "Criação dos tokens concluída com erro léxico:\n" << std::endl;
        tokens.imprimeErros();
        return std::nullopt;
    }

    try {

        Programa programa = prog(tokens);

        std::optional<Token> sobra = tokens.proximoToken();
        if (sobra.has_value()) {
            throw std::invalid_argument("Erro: expressão possui tokens extras ou parênteses desbalanceados.");
            return std::nullopt;
        }

        return programa;
   
    } 
    catch (const std::invalid_argument& e) {
        std::cerr << "\n- " << e.what() << std::endl;
        return std::nullopt;
    }
}

