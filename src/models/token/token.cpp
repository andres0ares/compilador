#include "token.h"
#include <iostream>
#include <string>
#include <map>
#include <cctype>

using namespace std;

// ================================== TIPO ====================================


// Mapeamento de strings para os valores do enum
map<Tipo, string> tipoParaString = {
    {Tipo::NUMERO, "Numero"},
    {Tipo::PAREN_ESQ, "ParenEsq"},
    {Tipo::PAREN_DIR, "ParenDir"},
    {Tipo::CHAVES_ESQ, "ChavesEsq"},
    {Tipo::CHAVES_DIR, "ChavesDir"},
    {Tipo::MENOR, "Menor"},
    {Tipo::MAIOR, "Maior"},
    {Tipo::EQUAL, "Equal"},
    {Tipo::IGUAL, "Igualdade"},
    {Tipo::PTVIRG, "PontoVirgula"},
    {Tipo::SUB, "Sub"},
    {Tipo::SOMA, "Soma"},
    {Tipo::DIV, "Div"},
    {Tipo::MULT, "Mult"},
    {Tipo::IF, "If"},
    {Tipo::ELSE, "Else"},
    {Tipo::WHILE, "While"},
    {Tipo::RETURN, "Return"},
    {Tipo::RETORNA, "retorna"},
    {Tipo::VARIAVEL, "Variavel"},
    {Tipo::INVALIDO, "Invalido"},
};


// ================================== Token ==================================

// Construtor
Token::Token(std::string lx, Tipo t, int o, int l, int c) : 
    lexema(lx),
    tipo(t),
    offset(o),
    linha(l), 
    coluna(c)
{}

Tipo Token::getTipo() const {
    return tipo;
}

std::string Token::getTipoAsString() const {
    return tipoParaString[tipo];
}

std::string Token::getValue() const {
    return lexema;
}

void Token::printToken() const {
    if(tipo == Tipo::INVALIDO) 
        std::cout << "ERROR: caractere inválido: < " << lexema << " > linha: " << linha << ", coluna: " << coluna << endl;
    else
        std::cout << "<" << tipoParaString[tipo] << ", '" << lexema << "', " << offset << ">" << endl;
}


