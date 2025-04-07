#ifndef PROGRAMA_H
#define PROGRAMA_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <optional>
#include "../Declaracao/Declaracao.h"

class Programa {
private:
    std::vector<Declaracao> declaracoes;
    std::optional<Declaracao> dec_final;
    size_t indiceAtual;

public:
    Programa();
    void adiciona(const Declaracao& declaracao);
    void adicionaFinal(const Declaracao& declaracao);
    void imprimeTokens() const;
    std::string gerar_codigo() const;
    std::string gerar_declaracoes() const;
};


#endif 