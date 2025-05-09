#ifndef PROGRAMA_H
#define PROGRAMA_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <optional>
#include "../Comando/Comando.h"
#include "../Expressao/Expressao.h"
#include "../Declaracao/Declaracao.h"

class Programa {
private:
    std::vector<Declaracao> declaracoes;
    std::vector<Comando*> comandos;
    Expressao* expressao_retorno;
    size_t indiceAtual;

public:
    Programa();
    std::set<std::string> getVariaveisDeclaradas() const;
    void adiciona(const Declaracao& declaracao);
    void adicionaExpressao(Expressao* expressao);
    void imprimeTokens() const;
    // bool variavelDeclarada(const std::string var) const;
    bool variavelDeclarada(const std::string& var) const;
    void adicionaComando(Comando* cmd);
    const std::vector<Comando*>& getComandos() const;
    std::string gerar_codigo() const;
    std::string gerar_declaracoes() const;
};


#endif 