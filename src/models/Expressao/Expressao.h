#pragma once
#include "../token/token.h"
#include <string>

class Expressao {
public:
    virtual int avaliar() const = 0;
    // virtual void imprimir() const = 0;
    virtual void imprimir(int nivel = 0) const = 0;
    virtual std::string gerar_codigo() const = 0;
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
};

// ========================================================================

class Identificador : public Expressao {
private:
    std::string valor;

public:
    explicit Identificador(std::string v);
    int avaliar() const override;
    void imprimir(int nivel) const override;
    std::string gerar_codigo() const override;
};


// ========================================================================

class OperacaoBin : public Expressao {
private:
    char operador;
    Expressao* esquerda;
    Expressao* direita;
    
public:
    OperacaoBin(char op, Expressao* esq, Expressao* dir);
    int avaliar() const override;
    void imprimir(int nivel) const override;
    std::string gerar_codigo() const override;
};

// ========================================================================

