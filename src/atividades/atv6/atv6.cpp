#include "atv6.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <cctype>
#include <filesystem>
#include <algorithm>

#include "../../models/token/token.h"
#include "../../models/tokens/tokens.h"
#include "../../models/Expressao/Expressao.h"
#include "../atv7/atv7.h"
#include "../atv5/atv5.h"
#include "../atv2/atv2.h"

using namespace std;


int executaCodigoAssembly() {
   
    if (system("as --64 -o ./output/compilado.o ./output/modelo.s") != 0) {
        std::cerr << "Erro ao montar o código assembly!\n";
        return 1;
    }

    if (system("ld -o compilado ./output/compilado.o") != 0) {
        std::cerr << "Erro ao ligar o código!\n";
        return 2;
    }

    if (system("./compilado") != 0) {
        std::cerr << "Erro ao executar o programa!\n";
        return 3;
    }

    return 0;
}



int Atv6::codigo_assembly(ifstream& file, bool test = false) {

    // Atv5 atv5;
    Atv7 atv7;
    Atv2 atv2;
  
    try {    
        // Construcao da análise sintatica
        std::optional<Programa> programa = atv7.analise_sintatico(file);

        if (!programa.has_value()) {
            // throw std::invalid_argument("Expressao Vazia!\n");
            return 1;
        }

        std::string codigo_assembly = atv2.gerarCodigoAssemblyComDeclaracao(programa->gerar_codigo(), programa->gerar_declaracoes());
        
        // if(test) {
        //     std::cout <<"\nValor esperado:" << std::endl;
        //     std::cout << (programa).avaliar() << "\n" << std::endl;
        //     std::cout << "\nCodigo assembly gerado:\n";
        // }

        // std::string codigo_assembly = atv2.gerarCodigoAssembly((programa).gerar_codigo());
        std::cout << codigo_assembly << std::endl;

        atv2.geraAssemblyFile(codigo_assembly);

        if(test) {
            std::cout <<"\nValor obtido: " << std::endl;
            executaCodigoAssembly();
        }
    
    } catch (const std::invalid_argument& e) {
        std::cerr << "\nErro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


int Atv6::testes_codigo_assembly(ifstream& file) {

    std::string path;

    std::cout << "\n-------- EXECUCAO DE TESTES --------\n";

    while (std::getline(file, path)) {

        path.erase(std::remove_if(path.begin(), path.end(), ::isspace), path.end());

        std::ifstream test_file;
        test_file.open(path); // Abre o arquivo separadamente
        
        if (!test_file.is_open()) {
            std::cerr << "Erro: Não foi possível abrir o arquivo: " << path << "\n";
            continue;  // Continua para o próximo arquivo
        }

        // Printa o conteúdo do arquivo antes de processá-lo
        std::cout << "\n\nConteudo de " << path << ":\n";
        std::string line;
        while (std::getline(test_file, line)) {
            std::cout << line << '\n';
        }
        std::cout << "------------------------\n";

        // Volta para o início do arquivo para processamento posterior
        test_file.clear();  
        test_file.seekg(0, std::ios::beg);

        codigo_assembly(test_file, true);
        
        // Aqui você pode processar o arquivo conforme necessário
        test_file.close();
    }

    return 0;

}

