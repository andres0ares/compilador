#ifndef ATV7_H
#define ATV7_H

#include <iostream>
#include <fstream>
#include <optional>
#include "../../models/tokens/Tokens.h"
#include "../../models/Expressao/Expressao.h"
#include "../../models/Programa/Programa.h"


class Atv7 {
public:
    std::optional<Programa> analise_sintatico(std::ifstream& file);
    // std::optional<Expressao*> get_arvore(std::ifstream& file);
    // std::optional<Expressao*> construirArvore(Tokens& token);
    Expressao* expAdv(Tokens& tokens);
    Expressao* expMult(Tokens& tokens);
    Expressao* expPrim(Tokens& tokens);
    Programa prog(Tokens& tokens);


};

#endif