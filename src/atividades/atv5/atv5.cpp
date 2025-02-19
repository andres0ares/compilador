#include "atv5.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <filesystem>
#include <map>
#include <cctype>

#include "../../models/token/token.h"
#include "../../models/tokens/tokens.h"
#include "../atv4/atv4.h"

using namespace std;
namespace fs = std::filesystem;

int Atv5::analise_sintatico(ifstream& file) {

    Atv4 atv4;
    
    //cria tokens do arquivos
    Tokens tokens = atv4.create_tokens(file);

    //se possuir erro lexico exibe o erro e retorna
    if(tokens.temTokenInvalido()) {
        std::cout << "Criacao dos tokens concluida com erro lexico: \n" << endl;
        tokens.imprimeErros();
        return 1;
    } 

    while (true) {

        //retorna o proximo token
        auto tokenOpt = tokens.proximoToken();

        // Se não tem mais tokens, retorna null e sai do loop
        if (!tokenOpt) { 
            break;
        }

        // obtem token
        Token token = tokenOpt.value();

        // imprime token
        // token.printToken(); 

        // obtem valor/lexema/string do token
        std::string value = token.getValue();

        // obtem tipo como string
        std::string tipo_string = token.getTipoAsString();

        //imprime lexema e tipo
        std::cout << "valor do token: '" << value << "', Tipo: " << tipo_string << endl;

        //verifica tipo do token
        Tipo tipo = token.getTipo();
        if(Tipo::NUMERO == tipo) 
            std::cout << "e uma constante" << endl
        ;

        //os tipos estao em /src/models/token/token.h

    }

    return 0;
}
