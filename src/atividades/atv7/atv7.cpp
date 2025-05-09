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
#include "../../models/Comando/Comando.h"
#include "../atv4/atv4.h"


using namespace std;

// <ex> ::= <exp_a> ((’<’ | ’>’ | ’==’)* <exp_a>)*
Expressao* Atv7::expressao(Tokens& tokens) {
    Expressao* esquerda = expAdv(tokens);

    while (true) {
        std::optional<Token> token = tokens.proximoToken();

        if (!token || (token->getTipo() != Tipo::MAIOR && token->getTipo() != Tipo::MENOR && token->getTipo() != Tipo::EQUAL)) {
            if (token) 
                tokens.retrocedeToken(); // devolve se não for operador válido
            break;
        }

        Expressao* direita = expAdv(tokens);

        if (!direita) {
            throw std::invalid_argument("Erro: expressão à direita de OPERADOR RELACIONAL inválida.");
        }
        esquerda = new OperacaoBin(token->getTipo(), esquerda, direita);
    }
    return esquerda;
}

//  <exp_a> ::= <exp_m> ((’+’ | ’-’) <exp_m>)*
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

        if (!direita) {
            throw std::invalid_argument("Erro: expressão à direita de operador '+' ou '-' inválida.");
        }
        esquerda = new OperacaoBin(token->getTipo(), esquerda, direita);
    }
    return esquerda;
}

// <exp_m> ::= <prim> ((’*’ | ’/’) <prim>)*
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
        if (!direita) {
            throw std::invalid_argument("Erro: expressão à direita de operador '*' ou '/' inválida.");
        }
        esquerda = new OperacaoBin(token->getTipo(), esquerda, direita);
    }
    return esquerda;
}

// <prim> ::= <num> | <var> | ’(’ <exp> ’)’
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

        return new OperacaoBin(token->getTipo(), new Constante(0), termo); // 0 - termo
    }
    
    // Identificação de número
    if (token->getTipo() == Tipo::NUMERO ) {
        try {
            int valor = std::stoi(token->getValue());
            return new Constante(valor);
        } catch (...) {
            throw std::invalid_argument("Erro: valor numérico inválido.");
        }
    }

    //Identificacao de variavel
    if (token->getTipo() == Tipo::VARIAVEL ) {
        return new Variavel(token->getValue());
    }

    // Abertura para nova expressão
    if (token->getTipo() == Tipo::PAREN_ESQ) {
        Expressao* exp = expressao(tokens);
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


Programa Atv7::prog(Tokens& tokens) {
    
    //cria objeto programa
    Programa programa;

    //le primeiro token
    std::optional<Token> token = tokens.proximoToken();
    std::optional<Token> var;
    Expressao* exp;

    //se vazio, retorna erro
    if (!token) {
        throw std::invalid_argument("Erro: expressão vazia ou token inesperado no final da entrada.\n");
    }

// Faz a Declaração das variáveis
    while(token->getTipo() == Tipo::VARIAVEL) {

        var = token; //salva token da variavel
        token = tokens.proximoToken(); //le proximo token

        //se nao for (=) retorna erro
        if (!token || token->getTipo() != Tipo::IGUAL) {
            token->printToken();
            throw std::invalid_argument("Erro: esperado '=' apos variavel.\n");
        }

        //le a expressao
        exp = expressao(tokens);

        //le proximo token e retorna erro se nao for ;
        token = tokens.proximoToken();

        if (!token || token->getTipo() != Tipo::PTVIRG) {
            token->printToken();
            throw std::invalid_argument("Erro: esperado ; após declaração.\n");
        }
        
        //adiciona declaracao
        Declaracao  declaracao(var.value(), exp);
        programa.adiciona(declaracao);

        token = tokens.proximoToken();
    }

    // Armazena os valores dos identificadores declarados
    std::set<std::string> declaradas = programa.getVariaveisDeclaradas();

// Iniciando a parte de Comandos
    if (!token || token->getTipo() != Tipo::CHAVES_ESQ) {
        throw std::invalid_argument("Erro: esperado '{' após declarações.");
    }
    
    // Loop de comandos até encontrar 'return'
    while (true) {
        token = tokens.proximoToken();

        if (!token)
            throw std::invalid_argument("Erro: fim inesperado ao procurar comandos ou 'return'.");

        if (token->getTipo() == Tipo::RETURN)
            break; // fim da lista de comandos

        if (token->getTipo() == Tipo::IF || token->getTipo() == Tipo::WHILE || token->getTipo() == Tipo::VARIAVEL) {
            tokens.retrocedeToken(); // retrocede para Comando::parse consumir corretamente
            Comando* cmd = Comando::parse(tokens, declaradas);
            programa.adicionaComando(cmd);
        } 
        else {
            token->printToken();
            throw std::invalid_argument("Erro: Ausência de Return, token inesperado na lista de comandos.");
        }
    }

    token = tokens.proximoToken();
    tokens.retrocedeToken();

    // Verifica a presença de Expressão de retorno
    if(token &&  token->getTipo() == Tipo::PTVIRG) {
        throw std::invalid_argument("Erro: Ausência de expressão retorno.\n");
    }

    //le expressao de Retorno
    exp = expressao(tokens);

    // Adiciona a expressao de retorno ao programa
    programa.adicionaExpressao(exp);

    token = tokens.proximoToken();
    // Se não for (;), retorna erro
    if (!token || token->getTipo() != Tipo::PTVIRG) {
        token->printToken();
        throw std::invalid_argument("Erro: esperado ; após expressão retorno.\n");
    }

    token = tokens.proximoToken();
    // Se não for (}), retorna erro
    if (!token || token->getTipo() != Tipo::CHAVES_DIR) {
        throw std::invalid_argument("Erro: esperado '}' ao final do programa.");
    }

    return programa;
}

std::optional<Programa> Atv7::analise_sintatico(ifstream& file) {
    Atv4 atv4;
    Tokens tokens = atv4.create_tokens(file);

    // tokens.imprimeTokensValidos();

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
    catch (const std::runtime_error& e) {
        std::cerr << "\n- " << e.what() << std::endl;
        return std::nullopt;
    }
}