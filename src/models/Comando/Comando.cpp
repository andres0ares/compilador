#include "../Programa/Programa.h"
#include "../token/token.h"
#include "Comando.h"
#include <iostream>
#include <string>

using namespace std;

// Removedor de espaços em branco a esquerda (para o compilador)
std::string _trimLeft(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : str.substr(start);
}


void verifica_variaveis_usadas(const Expressao* e, const std::set<std::string>& declaradas) {
    for (const auto& var : e->getVariaveis()) {
        if (!declaradas.count(var)) 
            throw std::runtime_error("Variável " + var + " usada sem declaração.");
    } 
}

Comando* Comando::parse(Tokens& tokens,  const std::set<std::string>& declaradas) {
    std::optional<Token> token = tokens.proximoToken();

    if (!token)
        throw std::invalid_argument("Erro: esperado comando.");

    switch (token->getTipo()) {
        case Tipo::IF:
            tokens.retrocedeToken();
            return If::parse(tokens, declaradas);

        case Tipo::WHILE:
            tokens.retrocedeToken();
            return While::parse(tokens, declaradas);

        case Tipo::VARIAVEL:
            tokens.retrocedeToken();
            return Atribuicao::parse(tokens, declaradas);

        default:
            token->printToken();
            throw std::invalid_argument("Erro: token inesperado em comando.");
    }
}

// =========================== IF ====================================


If::If(Expressao* cond, std::vector<Comando*> bIf, std::vector<Comando*> bElse) :
    condicao(cond),
    blocoIf(bIf),
    blocoElse(bElse)
{}

std::set<std::string> If::getVariaveis() const {
    std::set<std::string> ids = condicao->getVariaveis();

    for (const auto& cmd : blocoIf) {
        auto vars = cmd->getVariaveis();
        ids.insert(vars.begin(), vars.end());
    }

    for (const auto& cmd : blocoElse) {
        auto vars = cmd->getVariaveis();
        ids.insert(vars.begin(), vars.end());
    }

    return ids;
}

Comando* If::parse(Tokens& tokens,  const std::set<std::string>& declaradas) {
    // Espera: if <exp> { <comando>* } else { <comando>* }
    std::optional<Token> token = tokens.proximoToken(); // consome 'if'

    if (!token || token->getTipo() != Tipo::IF)
        throw std::invalid_argument("Esperado 'if'.");

    Expressao* cond = Expressao::parse(tokens);

    verifica_variaveis_usadas(cond, declaradas);

    token = tokens.proximoToken();
    if (!token || token->getTipo() != Tipo::CHAVES_ESQ)
        throw std::invalid_argument("Esperado '{' após if");

    std::vector<Comando*> comandosThen;
    while (true) {
        token = tokens.proximoToken();
        if (!token || token->getTipo() == Tipo::CHAVES_DIR)
            break;
        tokens.retrocedeToken();
        comandosThen.push_back(Comando::parse(tokens, declaradas));
    }

    token = tokens.proximoToken();
    if (!token || token->getTipo() != Tipo::ELSE)
        throw std::invalid_argument("Esperado 'else' após if");

    token = tokens.proximoToken();
    if (!token || token->getTipo() != Tipo::CHAVES_ESQ)
        throw std::invalid_argument("Esperado '{' após else");

    std::vector<Comando*> comandosElse;
    while (true) {
        token = tokens.proximoToken();
        if (!token || token->getTipo() == Tipo::CHAVES_DIR)
            break;
        tokens.retrocedeToken();
        comandosElse.push_back(Comando::parse(tokens, declaradas));
    }

    return new If(cond, comandosThen, comandosElse);
}


std::string If::gerar_codigo() const {
    std::string codigoCond = condicao->gerar_codigo();
    std::string codigoIf, codigoElse;

    for (const auto& cmd : blocoIf)
        codigoIf += cmd->gerar_codigo() + "\n";

    for (const auto& cmd : blocoElse)
        codigoElse += cmd->gerar_codigo() + "\n";

        // Indentificação qual é o conjunto de if e else
        static int id = 0;
        std::string labelElse = "else_" + std::to_string(id++);
        std::string labelFim = "fim_if_" + std::to_string(id++);

    return codigoCond + R"(
        cmp $0, %rax
        je )" + labelElse + R"(
        )" + _trimLeft(codigoIf) + R"(
        jmp )" + labelFim + R"(
        )" + labelElse + R"(: 
        )" + _trimLeft(codigoElse) + R"(
        )" + labelFim + R"(:)" + "\n";
}

Expressao* If::getExpressao() const{
    return condicao;
}

// =========================== WHILE ===========================

// Construtor
While::While(Expressao* cond, std::vector<Comando*> bWhile) :
    condicao(cond),
    blocoWhile(bWhile)
{}

std::set<std::string> While::getVariaveis() const {
    std::set<std::string> ids = condicao->getVariaveis();

    for (const auto& cmd : blocoWhile) {
        auto vars = cmd->getVariaveis();
        ids.insert(vars.begin(), vars.end());
    }
    return ids;
}

Comando* While::parse(Tokens& tokens,  const std::set<std::string>& declaradas) {
    // Espera: while <exp> { <comando>* }
    std::optional<Token> token = tokens.proximoToken(); // consome 'while'

    if (!token || token->getTipo() != Tipo::WHILE)
        throw std::invalid_argument("Esperado 'while'.");

    Expressao* cond = Expressao::parse(tokens);

    verifica_variaveis_usadas(cond, declaradas);

    token = tokens.proximoToken();
    if (!token || token->getTipo() != Tipo::CHAVES_ESQ)
        throw std::invalid_argument("Esperado '{' após while");

    std::vector<Comando*> comandos;
    while (true) {
        token = tokens.proximoToken();
        if (!token || token->getTipo() == Tipo::CHAVES_DIR)
            break;
        tokens.retrocedeToken();
        comandos.push_back(Comando::parse(tokens, declaradas));
    }

    return new While(cond, comandos);
}

std::string While::gerar_codigo() const {
    // Indentificação de qual loop está
    static int id = 0;
    std::string label = "loop" + std::to_string(id++);
    std::string inicio = label + "_inicio";
    std::string fim = label + "_fim";

    
    std::string cod = "\t" + inicio + ":\n";
    cod += condicao->gerar_codigo() + "\n";
    cod += "\tcmp $0, %rax\n\tje " + fim + "\n";

    for (Comando* c : blocoWhile) {
        cod += c->gerar_codigo() + "\n";
    }
    cod += "\tjmp " + inicio + "\n";
    cod += "\t" + fim + ":\n";
    return cod;
}

Expressao* While::getExpressao() const{
    return condicao;
}


// ========================== ATRIBUTO ==========================

// Construtor
Atribuicao::Atribuicao(Token t, Expressao* e)
    : token(t), expressao(e)
{}

Token Atribuicao::getToken() const {
    return token;
}

Expressao* Atribuicao::getExpressao() const {
    return expressao;
}

std::set<std::string> Atribuicao::getVariaveis() const {
    std::set<std::string> vars = expressao->getVariaveis();  // variáveis usadas na expressão
    vars.insert(token.getValue());                           // variável que está sendo atribuída
    return vars;
}


Comando* Atribuicao::parse(Tokens& tokens,  const std::set<std::string>& declaradas) {
    auto tokenVar = tokens.proximoToken();
    if (!tokenVar || tokenVar->getTipo() != Tipo::VARIAVEL) {
        throw std::runtime_error("Erro: esperado identificador de variável.");
    }

    // Verifica se a variavel da atribuiçao já foi declarada
    if (!declaradas.count(tokenVar->getValue())) {
        throw std::runtime_error("Variável '" + tokenVar->getValue() + "' atribuída sem ser declarada.");
    }

    auto tokenIgual = tokens.proximoToken();
    if (!tokenIgual || tokenIgual->getTipo() != Tipo::IGUAL) {
        throw std::runtime_error("Erro: esperado '=' após variável.");
    }

    Expressao* expr = Expressao::parse(tokens);

    verifica_variaveis_usadas(expr, declaradas);

    auto tokenPontoEVirgula = tokens.proximoToken();
    if (!tokenPontoEVirgula || tokenPontoEVirgula->getTipo() != Tipo::PTVIRG) {
        throw std::runtime_error("Erro: esperado ';' após expressão.");
    }

    return new Atribuicao(*tokenVar, expr); // já é um Comando*
}

std::string Atribuicao::gerar_codigo() const {
    return expressao->gerar_codigo() + "\n\tmov %rax, " + token.getValue();
}

Tipo Atribuicao::getTipo() const {
    return token.getTipo();
}

void Atribuicao::printToken() const {
    return token.printToken();
}


