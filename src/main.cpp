#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <filesystem>

#include "atividades/atv2/atv2.h"
#include "atividades/atv3/atv3.h"
#include "atividades/atv4/atv4.h"


using namespace std;
namespace fs = std::filesystem;

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

    // Atv2 atv2;
    // atv2.atv2_main(file);

    // Atv3 atv3;
    // atv3.atv3_main(file);

    Atv4 atv4;
    atv4.analise_lexico(file);

    return 0;
}