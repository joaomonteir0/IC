#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "ImageProcessor.cpp" 

void listarImagens(const std::string& caminho) {
    std::cout << "Imagens disponíveis:" << std::endl;
    for (const auto& entry : std::filesystem::directory_iterator(caminho)) {
        if (entry.is_regular_file()) {
            std::string extensao = entry.path().extension().string();
            if (extensao == ".ppm") {
                std::cout << entry.path().filename().string() << std::endl;
            }
        }
    }
}

std::string escolherImagem() {
    std::string imagemEscolhida;
    std::cout << "Digite o nome da imagem (incluindo a extensão .ppm): ";
    std::cin >> imagemEscolhida;
    return imagemEscolhida;
}

void menu() {
    std::cout << "Selecione uma opção:" << std::endl;
    std::cout << "1 - Carregar imagem" << std::endl;
    std::cout << "0 - Sair" << std::endl;
    int op;
    std::cin >> op;
    switch (op) {
        case 1: {
            // Carregar imagem
            listarImagens("./"); // Substitua pelo caminho real
            std::string image_path = escolherImagem(); // Obter a imagem escolhida

            ImageProcessor imgProc; // iniciar o objeto do ImageProcessor

            // mandar erro se não abrir a imagem 
            if (!imgProc.loadImage(image_path)) {
                std::cout << "Não foi possível carregar a imagem: " << image_path << std::endl;
                return; // Retornar ao menu após erro
            }

            // Mostrar a imagem
            imgProc.displayImage("Janela de Exibição");
            break; // Adicionado para evitar chamada ao menu após a escolha da imagem
        }
        case 0:
            return;
        default:
            std::cout << "Opção inválida." << std::endl;
    }
    menu(); // Chama a função novamente para mostrar o menu novamente após a escolha
}

int main() {
    menu(); // Inicia o menu
    return 0;
}
