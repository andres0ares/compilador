#ifndef ATV2_H
#define ATV2_H

#include <iostream>
#include <fstream> 

class Atv2 {
public:
    int atv2_main(std::ifstream& file);
    std::string gerarCodigoAssembly(const std::string &saida_compilador);
    int geraAssemblyFile(const std::string &codigo);
};

#endif