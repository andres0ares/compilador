#ifndef ATV5_H
#define ATV5_H

#include <iostream>
#include <fstream>
#include <optional>
#include "../../models/Expressao/Expressao.h"

class Atv5 {
public:
    int analise_sintatico(std::ifstream& file);
    std::optional<Expressao*> get_arvore(std::ifstream& file);
};

#endif