// #include <iostream>
// #include <string>

// class Atv5 {
// public:
//     int analise_sintatico(std::ifstream& file);
// };

// // Definição da classe-base Expressao (abstrata)
// class Expressao {
// public:
//     virtual int avaliar() const = 0; // Método virtual puro para avaliação da expressão
//     virtual void imprimir() const = 0; // Método virtual puro para imprimir a expressão
//     virtual ~Expressao() = default;
// };

// // Classe para representar constantes inteiras
// class Constante : public Expressao {
// private:
//     int valor;
// public:
//     explicit Constante(int v) : valor(v) {}
//     int avaliar() const override { return valor; }
//     void imprimir() const override { std::cout << valor; }
// };

// // Classe para representar operações binárias
// class OperacaoBin : public Expressao {
// private:
//     char operador;
//     Expressao* esquerda;
//     Expressao* direita;
// public:
//     OperacaoBin(char op, Expressao* esq, Expressao* dir)
//         : operador(op), esquerda(esq), direita(dir) {}

//     ~OperacaoBin() {
//         delete esquerda;
//         delete direita;
//     }

//     int avaliar() const override {
//         int valEsq = esquerda->avaliar();
//         int valDir = direita->avaliar();
//         switch (operador) {
//             case '+': return valEsq + valDir;
//             case '-': return valEsq - valDir;
//             case '*': return valEsq * valDir;
//             case '/': return valDir != 0 ? valEsq / valDir : 0; // Tratamento de divisão por zero
//             default: throw std::runtime_error("Operador desconhecido");
//         }
//     }

//     void imprimir() const override {
//         std::cout << "(";
//         esquerda->imprimir();
//         std::cout << " " << operador << " ";
//         direita->imprimir();
//         std::cout << ")";
//     }
// };

// int main() {
//     // Construção manual da árvore sintática para a expressão (3 + (4 * 5))
//     Expressao* expressao = new OperacaoBin('+',
//         new Constante(3),
//         new OperacaoBin('*',
//             new Constante(4),
//             new Constante(5)
//         )
//     );

//     // Imprime a expressão
//     std::cout << "Expressao: ";
//     expressao->imprimir();
//     std::cout << "\n";
    
//     // Avalia e imprime o resultado
//     std::cout << "Resultado: " << expressao->avaliar() << "\n";
    
//     // Libera memória
//     delete expressao;

//     return 0;
// }
