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
    {Tipo::SOMA, "Soma"},
    {Tipo::MULT, "Mult"},
    {Tipo::SUB, "Sub"},
    {Tipo::DIV, "Div"},
    {Tipo::INVALIDO, "invalido"},
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

void Token::printToken() const {
    if(tipo == Tipo::INVALIDO) 
        std::cout << "ERROR: caractere inválido: < " << lexema << " > linha: " << linha << ", coluna: " << coluna << endl;
    else
        std::cout << "<" << tipoParaString[tipo] << ", '" << lexema << "', " << offset << ">" << endl;
}


