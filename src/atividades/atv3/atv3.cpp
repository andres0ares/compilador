#include "atv3.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void Atv3::atv3_main(ifstream& file) {
    std::string codigo; 
    std::string linha; 

    while (std::getline(file, linha)) {
        codigo += "   " + linha + "\n";
    };
    file.close();

    std::cout << codigo << endl;

}
