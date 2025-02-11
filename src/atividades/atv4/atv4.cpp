#include "atv4.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <filesystem>
#include <map>
#include <cctype>

using namespace std;
namespace fs = std::filesystem;

enum class Tipo {
    NUMERO = 1,
    PAREN_ESQ = 2,
    PAREN_DIR = 3,
    SOMA = 4,
    MULT = 5,
    SUB = 6,
    DIV = 7,
    INVALIDO = 8,
};

// Mapeamento de strings para os valores do enum
map<Tipo, string> tipoParaString = {
    {Tipo::NUMERO, "Numero"},
    {Tipo::PAREN_ESQ, "ParenEsq"},
    {Tipo::PAREN_DIR, "ParenDir"},
    {Tipo::SOMA, "Soma"},
    {Tipo::MULT, "Mult"},
    {Tipo::SUB, "Sub"},
    {Tipo::DIV, "Div"},
    {Tipo::INVALIDO, "invalido"},
};

Tipo identificarTipo(char c) {
    if (isdigit(c)) return Tipo::NUMERO;
    if (c == '(') return Tipo::PAREN_ESQ;
    if (c == ')') return Tipo::PAREN_DIR;
    if (c == '+') return Tipo::SOMA;
    if (c == '-') return Tipo::SUB;
    if (c == '*') return Tipo::MULT;
    if (c == '/') return Tipo::DIV;
    return Tipo::INVALIDO;
}


class Token {
public:
    std::string lexema;
    Tipo tipo;
    int offset;
    int linha;
    int coluna;

    // Construtor
    Token(std::string lx, Tipo t, int o, int l, int c) : 
        lexema(lx),
        tipo(t),
        offset(o),
        linha(l), 
        coluna(c)
    {}

    void printToken() {
        if(tipo == Tipo::INVALIDO) 
            std::cout << "ERROR: caractere inválido: < " << lexema << " > linha: " << linha << ", coluna: " << coluna << endl;
        else
            std::cout << "<" << tipoParaString[tipo] << ", '" << lexema << "', " << offset << ">" << endl;
    }
};

void create_tokens(ifstream& file) {
//void create_tokens() {

    int off_set = -1;
    char caractere;
    int linha = 1;
    int coluna = 0;

    while (file.get(caractere)) { 

        off_set++;

        if (caractere == '\n') {
            linha++;
            coluna = 0; // Resetamos a coluna
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
            token.printToken();
        }
        else {
            // Identifica o tipo do caractere único
            Tipo tipo = identificarTipo(caractere);
            
            // Cria o token 
            Token token(string(1, caractere), tipo, off_set, linha, coluna);
            token.printToken();
        }

    }

    file.close(); // Fechando o arquivo
    
    // Token token1("(", Tipo::PAREN_ESQ, 0);
    // Token token2("2", Tipo::NUMERO, 1);
    // token1.printToken();
    // token2.printToken();

}

int Atv4::analise_lexico(ifstream& file) {

    std::cout << "lendo arquivo " << "output/modelo.s" << std::endl;
    create_tokens(file);

    return 0;
}
