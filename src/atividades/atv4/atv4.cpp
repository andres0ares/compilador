#include "atv4.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <filesystem>
#include <map>
#include <cctype>

#include "../../models/token/token.h"
#include "../../models/tokens/tokens.h"

using namespace std;
namespace fs = std::filesystem;

Tipo identificarTipo(char c) {
    if (isdigit(c)) return Tipo::NUMERO;
    if (c == '(') return Tipo::PAREN_ESQ;
    if (c == ')') return Tipo::PAREN_DIR;
    if (c == '{') return Tipo::CHAVES_ESQ;
    if (c == '}') return Tipo::CHAVES_DIR;
    if (c == '<') return Tipo::MENOR;
    if (c == '>') return Tipo::MAIOR;
    if (c == '=') return Tipo::IGUAL;
    if (c == ';') return Tipo::PTVIRG;
    if (c == '-') return Tipo::SUB;
    if (c == '+') return Tipo::SOMA;
    if (c == '/') return Tipo::DIV;
    if (c == '*') return Tipo::MULT;

    return Tipo::INVALIDO;
}

Tokens Atv4::create_tokens(ifstream& file) {

    int off_set = -1;
    char caractere;
    int linha = 1;
    int coluna = 0;
    Tokens tokens_list;

    while (file.get(caractere)) { 

        off_set++;

        if (caractere == '\n') {
            linha++;
            coluna = 0; 
            continue;
        }

        coluna++;

        if (isspace(caractere)) {
            continue; // Ignora espaços em branco
        }

        // Se for um número, precisa capturar o número inteiro
        if (isdigit(caractere)) {
            string numero(1, caractere);
            int inicio_offset = off_set; 
            
            // Lê o restante dos dígitos para formar o número completo
            while (file.get(caractere) && isdigit(caractere)) {
                numero += caractere;
                off_set++; 
            }

            //identificador invalido
            if(isalpha(caractere)){
                // if(identificarTipo(caractere) == Tipo::INVALIDO && !isspace(caractere)) {

                numero += caractere;

                while (file.get(caractere) && !isspace(caractere) && identificarTipo(caractere) == Tipo::INVALIDO) {
                    numero += caractere;
                    off_set++; 
                    coluna++;
                }

                // Volta um caractere pois leu um a mais no último `file.get`
                file.unget();

                Token token(numero, Tipo::INVALIDO, inicio_offset, linha, coluna);
                tokens_list.adiciona(token);

            }else{
                // Volta um caractere pois leu um a mais no último `file.get`
                file.unget();

                // Cria o token para o número
                Token token(numero, Tipo::NUMERO, inicio_offset, linha, coluna);
                tokens_list.adiciona(token);
            } 

            continue;
        }

        if(identificarTipo(caractere) != Tipo::INVALIDO) {
            int inicio_offset = off_set; 

            if(caractere == '=') {
                string equal(1, caractere);
                file.get(caractere);

                // Confere se o operador será ==
                if(caractere == '=') {
                    off_set++;
                    coluna++;
                    equal += caractere;
                    Token token(equal, Tipo::EQUAL, inicio_offset, linha, coluna);
                    tokens_list.adiciona(token);
                    
                }else {
                    Token token("=", Tipo::IGUAL, off_set, linha, coluna);
                    // Volta um caractere pois leu um a mais no último `file.get`
                    file.unget();
                    tokens_list.adiciona(token);
                }

                continue;
            }

            // Cria o token 
            Token token(string(1, caractere), identificarTipo(caractere), off_set, linha, coluna);
            tokens_list.adiciona(token);

            continue;
        }

        if(isalpha(caractere)) {
            string palavra(1, caractere);
            int inicio_offset = off_set; 
        
            // Lê o restante
            while (file.get(caractere) && isalpha(caractere) ) {
                palavra += caractere;
                off_set++; 
                coluna++; 
            }

            // Se houver qualquer outro caractere colado (número ou símbolo), é inválido
            if (isalnum(caractere) || caractere == '_') {
                palavra += caractere;
                while (file.get(caractere) && !isspace(caractere) && identificarTipo(caractere) == Tipo::INVALIDO) {
                    palavra += caractere;
                    off_set++;
                    coluna++;
                }
                // Volta um caractere lido a mais
                file.unget();
                Token token(palavra, Tipo::INVALIDO, inicio_offset, linha, coluna);
                tokens_list.adiciona(token);
                continue;
            }

            // Se leu um caractere que não é parte da variavel, volta
            if (!isspace(caractere) && identificarTipo(caractere) != Tipo::INVALIDO)
                file.unget();

            if(palavra == "if") {
                Token token(palavra, Tipo::IF, inicio_offset, linha, coluna);
                tokens_list.adiciona(token);

            }else if(palavra == "else") {
                Token token(palavra, Tipo::ELSE, inicio_offset, linha, coluna);
                tokens_list.adiciona(token);

            }else if(palavra == "while") {
                Token token(palavra, Tipo::WHILE, inicio_offset, linha, coluna);
                tokens_list.adiciona(token);

            }else if(palavra == "return") {
                Token token(palavra, Tipo::RETURN, inicio_offset, linha, coluna);
                tokens_list.adiciona(token);

            }else{
                Token token(palavra, Tipo::VARIAVEL, inicio_offset, linha, coluna);
                tokens_list.adiciona(token);
            }
        }
    }

    file.close(); // Fechando o arquivo
    return tokens_list;
}

int Atv4::analise_lexico(ifstream& file) {
    //cria tokens do arquivos
    Tokens tokens = create_tokens(file);


    if(tokens.temTokenInvalido()) {
        std::cout << "Criacao dos tokens concluida com erro lexico: \n" << endl;
        tokens.imprimeErros();
        std::cout << "\ntokens validos: \n" << endl;
        tokens.imprimeTokensValidos();
    } else {
        tokens.imprimeTokens();
    }


    return 0;
}
