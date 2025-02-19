#ifndef ATV4_H
#define ATV4_H

#include <iostream>
#include <fstream>
#include "../../models/tokens/tokens.h" 

class Atv4 {
public:
    int analise_lexico(std::ifstream& file);
    Tokens create_tokens(std::ifstream& file);
};

#endif