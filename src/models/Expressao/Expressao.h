#pragma once
#include <set>
#include <string>
#include "../token/token.h"
#include "../tokens/tokens.h"

class Expressao {
public:
    static Expressao* parse(Tokens& tokens);
    virtual int avaliar() const = 0;
    // virtual void imprimir() const = 0;
    virtual void imprimir(int nivel = 0) const = 0;
    virtual std::string gerar_codigo() const = 0;
    virtual std::set<std::string> getVariaveis() const = 0;
};

// ========================================================================

class Constante : public Expressao {
private:
    int valor;

public:
    explicit Constante(int v);
    int avaliar() const override;
    void imprimir(int nivel) const override;
    std::string gerar_codigo() const override;
    std::set<std::string> getVariaveis() const override;
};

// ========================================================================

class Variavel : public Expressao {
private:
    std::string valor;

public:
    explicit Variavel(std::string v);
    int avaliar() const override;
    void imprimir(int nivel) const override;
    std::string gerar_codigo() const override;
    std::set<std::string> getVariaveis() const override;
};


// ========================================================================

class OperacaoBin : public Expressao {
private:
    Tipo operador;
    Expressao* esquerda;
    Expressao* direita;
    
public:
    OperacaoBin(Tipo op, Expressao* esq, Expressao* dir);
    int avaliar() const override;
    void imprimir(int nivel) const override;
    std::string gerar_codigo() const override;
    std::set<std::string> getVariaveis() const override;
};

// ========================================================================

