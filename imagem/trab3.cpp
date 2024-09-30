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
    std::cout << "2 - Visualizar canais de cor & grayscale" << std::endl;
    std::cout << "0 - Sair" << std::endl;

    int op;
    std::cin >> op;

    static ImageProcessor imgProc; // Persistent across menu calls

    switch (op) {
        case 1: {
            listarImagens("./"); // List available images in the current directory
            std::string image_path = escolherImagem(); // Get the selected image

            if (!imgProc.loadImage(image_path)) {
                std::cout << "Não foi possível carregar a imagem: " << image_path << std::endl;
                return;
            }
            imgProc.displayImage("Imagem Original");
            break;
        }
        case 2: {
            imgProc.displayChannels();  // Display color channels
            break;
        }
        case 0:
            return;
        default:
            std::cout << "Opção inválida." << std::endl;
    }
    menu(); 
}

int main() {
    menu(); // Inicia o menu
    return 0;
}
