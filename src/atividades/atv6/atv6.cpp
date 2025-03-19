#include "atv6.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <cctype>

#include "../../models/token/token.h"
#include "../../models/tokens/tokens.h"
#include "../../models/Expressao/Expressao.h"
#include "../atv5/atv5.h"

using namespace std;



int Atv6::codigo_assembly(ifstream& file) {

    Atv5 atv5;
  
    try {    
        //construcao da arvore sintatica
        std::optional<Expressao*> raiz = atv5.get_arvore(file);

        if (!raiz.has_value()) {
            throw std::invalid_argument("Expressao Vazia!\n");
        }

        std::cout << (*raiz)->gerar_codigo() << std::endl;
    
        } catch (const std::invalid_argument& e) {
            std::cerr << "\nErro: " << e.what() << std::endl;
            return 1;
        }

    return 0;
}

