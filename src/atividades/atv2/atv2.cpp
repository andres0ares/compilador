#include "atv2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Função que gera o código Assembly completo a partir da string fornecida
std::string gerarCodigoAssembly(const std::string &saida_compilador) {
    std::string modelo_assembly = R"(
        .section .text
        .globl _start

        _start:
        )" + saida_compilador + R"( 

        call imprime_num
        call sair

        .include "./output/runtime.s"
    )";

    return modelo_assembly;
}

std::string valida(ifstream& file) {
    // Leitura da primeira linha do arquivo
    std::string linha;
    getline(file, linha);  
    file.close();

    int linguagem;

    // Validação da linguagem (numero inteiro)
    try {
        size_t pos;
        linguagem = stoi(linha, &pos);  // Converte a linha para inteiro

        // Verifica se toda a linha foi convertida (sem caracteres extras)
        if (pos != linha.size()) {
            cout << "Erro: O arquivo contém caracteres não numéricos." << endl;
            return "";
        }

    } catch (const invalid_argument& e) {
        cout << "Erro: O arquivo não contém um número inteiro válido." << endl;
        return "";
    } catch (const out_of_range& e) {
        cout << "Erro: O número no arquivo está fora do intervalo válido para um inteiro." << endl;
        return "";
    }

    // Cria uma string formatada (assembly)
    ostringstream oss;
    oss << "mov $" << linguagem << ", %rax";

    return oss.str();
}

int Atv2::atv2_main(ifstream& file) {

    std::string linguagem_assembly = valida(file);
    
    if (linguagem_assembly.empty()) {
        return 1;
    }

    // Gera o código Assembly completo conforme modelo
    std::string codigo_assembly = gerarCodigoAssembly(linguagem_assembly);

    // Salva código Assembly 
    std::ofstream arquivo("output/modelo.s");

    if (arquivo.is_open()) {
      
        arquivo << codigo_assembly; 
        arquivo.close();

        std::cout << "Codigo assembly salvo em " << "output/modelo.s" << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo para escrita!" << std::endl;
    }

    // Caminho do arquivo de origem (runtime.s no diretório assembly)
    fs::path origem = fs::current_path() / "src" / "assembly" / "runtime.s";
    fs::path destino = fs::current_path() / "output" / "runtime.s";
    fs::copy(origem, destino, fs::copy_options::overwrite_existing);

    return 0;
}


