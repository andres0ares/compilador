#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <filesystem>

#include "atividades/atividade1/atividade1.h"

using namespace std;
namespace fs = std::filesystem;


std::string segundaAtividade(ifstream& file) {
    std::string codigo; 
    std::string linha; 

    while (std::getline(file, linha)) {
        codigo += "   " + linha + "\n";
    };
    file.close();
    return codigo;
}


int main(int argc, char* argv[]) {

    // Verifica se os argumentos foram passados
    if (argc < 2) {
        cout << "Arquivo de entrada nao informado" << endl;
        return 1; 
    }

    // Validação de arquivo
    std::string filename = argv[1];
    ifstream file(filename);

    if (!file) {
        cout << "Erro ao abrir o arquivo: " << filename << endl;
        return 1;
    }

    Atividade1 atividade1;
    atividade1.primeiraAtividade(file);

    return 0;
}