#ifndef ATV6_H
#define ATV6_H

#include <iostream>
#include <fstream>

class Atv6 {
public:
    int codigo_assembly(std::ifstream& file, bool test);
    int testes_codigo_assembly(std::ifstream& file);
};

#endif