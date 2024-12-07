#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

// Funcao que gera o código Assembly completo a partir da string fornecida
std::string gerarCodigoAssembly(const std::string& saida_compilador) {
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


int main(int argc, char* argv[]) {
    
    // verifica se argumento foi passado
    if (argc < 2) {
        cout << "Arquivo de entrada nao informado" << endl;
        return 1; 
    }

    // valida arquivo
    std::string filename = argv[1];
    ifstream file(filename);

    if (!file) {
        cout << "Erro ao abrir o arquivo: " << filename << endl;
        return 1;
    }
    
    // le arquivo
    std::string linha;
    getline(file, linha);  
    file.close();

    int linguagem;


    // valida linguagem (numero inteiro)
    try {
        size_t pos;
        linguagem = stoi(linha, &pos);  // Converte a linha para inteiro

        // Verifica se toda a linha foi convertida (sem caracteres extras)
        if (pos != linha.size()) {
            cout << "Erro: O arquivo contém caracteres não numéricos." << endl;
            return 1;
        }

    } catch (const invalid_argument& e) {
        cout << "Erro: O arquivo não contém um número inteiro válido." << endl;
        return 1;
    } catch (const out_of_range& e) {
        cout << "Erro: O número no arquivo está fora do intervalo válido para um inteiro." << endl;
        return 1;
    }

    //converte para assembly
    ostringstream oss;
    oss << "mov $" << linguagem << ", %rax";

    std::string linguagem_assembly = oss.str();

    // Gerando o código Assembly completo conforme modelo
    std::string codigo_assembly = gerarCodigoAssembly(linguagem_assembly);

    // salva código Assembly 
    std::ofstream arquivo("output/modelo.s");

    if (arquivo.is_open()) {
      
        arquivo << codigo_assembly; 
        arquivo.close();

        std::cout << "codigo assembly salvo em " << "output/modelo.s" << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo para escrita!" << std::endl;
    }

    // Caminho do arquivo de origem (runtime.s no diretório assembly)
    fs::path origem = fs::current_path() / "src" / "assembly" / "runtime.s";
    fs::path destino = fs::current_path() / "output" / "runtime.s";
    fs::copy(origem, destino, fs::copy_options::overwrite_existing);

    return 0;
}