#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <map>
#include <string>

// Definição dos tipos de tokens
enum class Tipo {
    NUMERO,
    PAREN_ESQ,
    PAREN_DIR,
    SOMA,
    SUB,
    MULT,
    DIV,
    INVALIDO
};

// Mapeamento Tipo → String
extern std::map<Tipo, std::string> tipoParaString;

// Classe Token
class Token {
private:
    std::string lexema;
    Tipo tipo;
    int offset;
    int linha;
    int coluna;

public:
    Token(std::string lx, Tipo t, int o, int l, int c);
    Tipo getTipo() const; // retorna tipo do token
    std::string getTipoAsString() const;
    std::string getValue() const;
    void printToken() const; // imprime token
};

#endif // TOKEN_H