#include "../../atividades/atv7/atv7.h"
#include "../token/token.h"
#include "Expressao.h"
#include <iostream>
#include <string>


Expressao* Expressao::parse(Tokens& tokens) {
    return Atv7::expressao(tokens); // delega a responsabilidade
}

Constante::Constante(int v) : 
    valor(v) 
{}

int Constante::avaliar() const {
    return valor;
}

void Constante::imprimir(int nivel = 0) const {
    std::cout << std::string(nivel * 4, ' ') << valor << std::endl;
}

std::set<std::string> Constante::getVariaveis() const {
    return {}; // nenhuma variável
}

// =======================================================================

Variavel::Variavel(std::string v) : 
    valor(v) 
{}

int Variavel::avaliar() const {
    return 0;
}

void Variavel::imprimir(int nivel = 0) const {
    std::cout << std::string(nivel * 4, ' ') << valor << std::endl;
}

// Retorna o label da variavel
std::set<std::string> Variavel::getVariaveis() const {
    return { valor };
}

// ========================================================================

OperacaoBin::OperacaoBin(Tipo op, Expressao* esq, Expressao* dir) :
    operador(op),
    esquerda(esq), 
    direita(dir) 
{}


int OperacaoBin::avaliar() const {
    int valorEsq = esquerda->avaliar();
    int valorDir = direita->avaliar();

    switch (operador) {
        case Tipo::SOMA: 
            return valorEsq + valorDir;
        case Tipo::SUB: 
            return valorEsq - valorDir;
        case Tipo::MULT: 
            return valorEsq * valorDir;
        case Tipo::DIV: 
            return valorDir != 0 ? valorEsq / valorDir : throw std::invalid_argument("Divisao por 0");
        case Tipo::MENOR:
            return valorEsq < valorDir;
        case Tipo::MAIOR:
            return valorEsq > valorDir;
        case Tipo::EQUAL:
            return valorEsq == valorDir;

        default: throw std::runtime_error("Operador desconhecido");
    }
}


//print da arvore de forma horizontal
void OperacaoBin::imprimir(int nivel = 0) const {
    std::string op;

    if(operador == Tipo::SOMA) op = "+";
    else if(operador == Tipo::SUB) op = "-";
    else if(operador == Tipo::MULT) op = "*";
    else if(operador == Tipo::DIV) op = "/";
    else if(operador == Tipo::MENOR) op = "<";
    else if(operador == Tipo::MAIOR) op = "+";
    else if(operador == Tipo::EQUAL) op = "==";

    direita->imprimir(nivel + 1);
    std::cout << std::string(nivel * 4, ' ') << op << std::endl;
    esquerda->imprimir(nivel + 1);
}

// Recursividade para buscar como a árvore está montada retorna (set)
std::set<std::string> OperacaoBin::getVariaveis() const {
    auto esqVars = esquerda->getVariaveis();
    auto dirVars = direita->getVariaveis();
    esqVars.insert(dirVars.begin(), dirVars.end());
    return esqVars;
}


// ============== GERAR CODIGO ============================

// Removedor de espaços
std::string trimLeft(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : str.substr(start);
}

// Para operações aritiméticas
std::string operador_assembly(Tipo op) {
    switch (op) {
        case Tipo::SOMA: 
            return "add %rbx, %rax";

        case Tipo::SUB: 
            return R"( 
        sub %rax, %rbx
        mov %rbx, %rax
        )";

        case Tipo::MULT: 
            return "imul %rbx, %rax";

        case Tipo::DIV: 
            return R"( 
        mov %rbx, %rcx
        mov %rax, %rbx
        mov %rcx, %rax
        idiv %rbx
        )";

        default: throw std::runtime_error("Operador desconhecido");
    }
}

// ========================================================================

std::string OperacaoBin::gerar_codigo() const {

    std::string op_esq = esquerda->gerar_codigo();
    std::string op_dir = direita->gerar_codigo();
        
    // Para operações aritméticas (+,-,*,/)
    if (operador == Tipo::SOMA || operador == Tipo::SUB || operador == Tipo::MULT || operador == Tipo::DIV) {
        std::string operacao = operador_assembly(operador); 

        return "\t\t" + trimLeft(op_esq) + R"(
        push %rax
        )" + trimLeft(op_dir) + R"( 
        pop %rbx
        )" + operacao;
    }

    // Para operações de Controle (<, >, ==)
    else {
        std::string cmp_instrucao;

        if (operador == Tipo::MENOR) {
            cmp_instrucao = R"(
        push %rax
        )" + trimLeft(op_dir) + R"(
        pop %rbx
        xor %rcx, %rcx
        cmp %rax, %rbx
        setl %cl
        mov %rcx, %rax
        )";

        } else if (operador == Tipo::MAIOR) {
            cmp_instrucao = R"(
        push %rax
        )" + trimLeft(op_dir) + R"(
        pop %rbx
        xor %rcx, %rcx
        cmp %rax, %rbx
        setg %cl
        mov %rcx, %rax
        )";
            
        } else if (operador == Tipo::EQUAL) {
            cmp_instrucao = R"(
        push %rax
        )" + trimLeft(op_dir) + R"(
        pop %rbx
        xor %rcx, %rcx
        cmp %rax, %rbx
        sete %cl
        mov %rcx, %rax
        )";
        }
        return op_esq + cmp_instrucao;
    }
}

std::string Constante::gerar_codigo() const {
    return "\tmov $" + std::to_string(valor) + ", %rax\n";
}

std::string Variavel::gerar_codigo() const {
    return "\tmov " + valor + ", %rax\n";
}

// ========================================================================
