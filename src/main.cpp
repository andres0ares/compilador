#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <filesystem>

#include "atividades/atv2/atv2.h"
#include "atividades/atv3/atv3.h"
#include "atividades/atv4/atv4.h"
#include "atividades/atv5/atv5.h"
#include "atividades/atv6/atv6.h"
#include "atividades/atv7/atv7.h"

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

    // Atv4 atv4;
    // atv4.analise_lexico(file);

    // Atv5 atv5;
    // atv5.analise_sintatico(file);

    // Atv6 atv6;
    // atv6.codigo_assembly(file);

    // GERADOR PARA ATIVIDADE 7!!
    Atv6 atv6;
    atv6.testes_codigo_assembly(file);

    return 0;
}