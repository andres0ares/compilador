#include "atv6.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <cctype>
#include <filesystem>

#include "../../models/token/token.h"
#include "../../models/tokens/tokens.h"
#include "../../models/Expressao/Expressao.h"
#include "../atv5/atv5.h"
#include "../atv2/atv2.h"

using namespace std;



int Atv6::codigo_assembly(ifstream& file, bool show_value = false) {

    Atv5 atv5;
    Atv2 atv2;
  
    try {    
        //construcao da arvore sintatica
        std::optional<Expressao*> raiz = atv5.get_arvore(file);

        if (!raiz.has_value()) {
            throw std::invalid_argument("Expressao Vazia!\n");
        }

        if(show_value) {
            std::cout <<"\nValor esperado:" << std::endl;
            std::cout << (*raiz)->avaliar() << "\n" << std::endl;
            std::cout << "\nCodigo assembly gerado:\n";
        }


        std::string codigo_assembly = atv2.gerarCodigoAssembly((*raiz)->gerar_codigo());
        std::cout << codigo_assembly << std::endl;

        atv2.geraAssemblyFile(codigo_assembly);
    
    } catch (const std::invalid_argument& e) {
        std::cerr << "\nErro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int executaCodigoAssembly() {
    return 0
}

int Atv6::testes_codigo_assembly(ifstream& file) {

    std::string path;

    while (std::getline(file, path)) {

        std::ifstream test_file(path);
        
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

