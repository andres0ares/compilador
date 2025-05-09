#pragma once
#include "../Expressao/Expressao.h"
#include "../Programa/Programa.h"
#include "../token/token.h"
#include "../tokens/tokens.h"
#include <optional>
#include <string>
#include <vector>
#include <set>

class Comando {
public:
    static Comando* parse(Tokens& tokens, const std::set<std::string>& declaradas);
    virtual std::set<std::string> getVariaveis() const = 0;
    virtual std::string gerar_codigo() const = 0;
    virtual Expressao* getExpressao() const = 0;
};

// =================================================================

class If : public Comando {
private:
    Expressao* condicao;
    std::vector<Comando*> blocoIf;
    std::vector<Comando*> blocoElse;

public:
    If(Expressao* cond, std::vector<Comando*> bIf, std::vector<Comando*> bElse);
    // Usasda para fazer a distinção em Comando
    static Comando* parse(Tokens& tokens, const std::set<std::string>& declaradas);
    std::set<std::string> getVariaveis() const override;
    std::string gerar_codigo() const override;
    Expressao* getExpressao() const override;

};

// =================================================================

class While : public Comando {
private:
    Expressao* condicao;
    std::vector<Comando*> blocoWhile;

public:
    While(Expressao* cond, std::vector<Comando*> bWhile);
    // Usasda para fazer a distinção em Comando
    static Comando* parse(Tokens& tokens, const std::set<std::string>& declaradas);
    std::set<std::string> getVariaveis() const override;
    std::string gerar_codigo() const override;
    Expressao* getExpressao() const override;
};

// =================================================================

class Atribuicao : public Comando {
private:
    Token token;
    Expressao* expressao;

public:
    Atribuicao(Token t, Expressao* e);
    // Usasda para fazer a distinção em Comando
    static Comando* parse(Tokens& tokens, const std::set<std::string>& declaradas);
    Token getToken() const;
    Tipo getTipo() const;
    void printToken() const; // imprime token
    std::set<std::string> getVariaveis() const override;
    std::string gerar_codigo() const override;
    Expressao* getExpressao() const override;
};