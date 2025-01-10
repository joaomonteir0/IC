#include "Golomb.h"
#include "BitStream.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

int main() {
    // Parâmetro M para Golomb
    int M = 10; // Pode ajustar para outro valor, se necessário

    // Criar um objeto Golomb
    Golomb golomb;

    // Vetores para armazenar valores
    std::vector<int> valores;
    std::vector<std::string> codificados;
    std::vector<int> descodificados;

    // Ler valores do ficheiro 'input.txt'
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Erro ao abrir input.txt!" << std::endl;
        return 1;
    }

    int num;
    while (inputFile >> num) {
        valores.push_back(num); // Adicionar valor ao vetor
    }
    inputFile.close(); // Fechar ficheiro

    // Codificar os valores
    std::cout << "Codificando valores:" << std::endl;
    for (int val : valores) {
        std::string code = golomb.encode(val, M); // Codifica com M=10
        codificados.push_back(code);
        std::cout << "Valor: " << val << " -> Código: " << code << std::endl;
    }

    // Descodificar os códigos
    std::cout << "\nDescodificando valores:" << std::endl;
    for (const std::string &code : codificados) {
        std::vector<int> dec = golomb.decode(code, M); // Descodifica com M=10
        int val = dec[0]; // Pega o primeiro valor do vetor retornado
        descodificados.push_back(val);
        std::cout << "Código: " << code << " -> Valor: " << val << std::endl;
    }

    // Validar se os valores descodificados correspondem aos originais
    if (valores == descodificados) {
        std::cout << "\nTeste bem-sucedido! Os valores descodificados correspondem aos originais." << std::endl;
    } else {
        std::cout << "\nErro no teste! Valores descodificados não correspondem." << std::endl;
    }

    return 0;
}
