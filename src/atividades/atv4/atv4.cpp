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
    if (c == '=') return Tipo::IGUAL;
    if (c == ';') return Tipo::PTVIRG;
    if (c == ')') return Tipo::PAREN_DIR;
    if (c == '+') return Tipo::SOMA;
    if (c == '-') return Tipo::SUB;
    if (c == '*') return Tipo::MULT;
    if (c == '/') return Tipo::DIV;
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

            // Volta um caractere pois leu um a mais no último `file.get`
            file.unget();

            // Cria o token para o número
            Token token(numero, Tipo::NUMERO, inicio_offset, linha, coluna);
            tokens_list.adiciona(token);
        }
        else {
            // Identifica o tipo do caractere único
            Tipo tipo = identificarTipo(caractere);

            //pode ser var ou return
            if(tipo == Tipo::INVALIDO) {

                string palavra(1, caractere);
                int inicio_offset = off_set; 
            
                // Lê o restante
                while (file.get(caractere) && !isspace(caractere) && identificarTipo(caractere) == Tipo::INVALIDO ) {
                    palavra += caractere;
                    off_set++; 
                }

                // Volta um caractere pois leu um a mais no último `file.get`
                file.unget();

                if(palavra == "retorna") {
                    Token token(palavra, Tipo::RETORNA, inicio_offset, linha, coluna);
                    tokens_list.adiciona(token);
                }else{
                    Token token(palavra, Tipo::IDENTIFICADOR, inicio_offset, linha, coluna);
                    tokens_list.adiciona(token);
                }

            }else{
                // Cria o token 
                Token token(string(1, caractere), tipo, off_set, linha, coluna);
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
