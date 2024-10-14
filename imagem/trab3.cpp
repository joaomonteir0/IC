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
    std::cout << "Select an option:" << std::endl;
    std::cout << "1 - Load an imagem" << std::endl;
    std::cout << "2 - Visualize the image RGB channels & grayscale" << std::endl;
    std::cout << "3 - Visualize the grayscale histogram" << std::endl;
    std::cout << "4 - Save the modified image" << std::endl;  // Implementar salvamento de imagens
    std::cout << "5 - Convert the image to another format" << std::endl;  // Implementar conversão de formatos
    std::cout << "6 - Apply a filter (e.g., grayscale, blur)" << std::endl;
    std::cout << "0 - Leave" << std::endl;

    int op;
    std::cin >> op;

    static ImageProcessor imgProc; // manter a imagem pelo menu

    switch (op) {
        case 1: {
            listarImagens("./"); // List available images in the current directory
            std::string image_path = escolherImagem(); // Get the selected image

            if (!imgProc.loadImage(image_path)) {
                std::cout << "Não foi possível carregar a imagem: " << image_path << std::endl;
                return;
            }
            imgProc.displayImage("Original");
            break;
        }
        case 2: {
            imgProc.displayChannels();  // Display color channels
            break;
        }
        case 3: {
            imgProc.displayHistogram(); // Display histogram
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
