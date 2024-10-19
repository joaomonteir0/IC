#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <filesystem>
#include "ImageProcessor.cpp"

// Functions to handle menu operations
void listarImagens(const std::string& caminho) {
    std::cout << "Available images:" << std::endl;
    for (const auto& entry : std::filesystem::directory_iterator(caminho)) {
        if (entry.is_regular_file()) {
            std::string extensao = entry.path().extension().string();
            if (extensao == ".ppm") {
                std::cout << entry.path().filename().string() << std::endl;
            }
        }
    }
}

// Function to choose an image
std::string escolherImagem() {
    std::string imagemEscolhida;
    std::cout << "Enter the image name (including the .ppm extension): ";
    std::cin >> imagemEscolhida;
    return imagemEscolhida;
}

// Function to handle the menu for one image
void menuPrincipalImagem() {
    std::cout << "Select an option:" << std::endl;
    std::cout << "1 - Load an image" << std::endl;
    std::cout << "2 - Generate the histogram (Gnuplot)" << std::endl;
    std::cout << "3 - Apply Gaussian Filter" << std::endl;
    std::cout << "4 - Return to the main menu" << std::endl;
    std::cout << "0 - Exit" << std::endl;

    int op;
    std::cin >> op;

    static ImageProcessor imgProc;

    switch (op) {
        case 1: {
            listarImagens("./");
            std::string caminhoImagem = escolherImagem();

            if (!imgProc.loadImage(caminhoImagem)) {
                std::cout << "Unable to load the image: " << caminhoImagem << std::endl;
                return;
            }
            std::cout << "Image successfully loaded!" << std::endl;
            break;
        }
        case 2: {
            imgProc.plotHistogramImage();
            break;
        }
        case 3: {
            int kernelSize;
            std::cout << "Enter the kernel size for the Gaussian filter (odd number >= 3): ";
            std::cin >> kernelSize;
            imgProc.applyGaussianFilter(kernelSize);  // Apply the filter
            break;
        }
        case 4: {
            return;  // Return to the main menu
        }
        case 0:
            return;
        default:
            std::cout << "Invalid option." << std::endl;
    }

    menuPrincipalImagem(); 
}

// Main menu function
void menuPrincipal() {
    int opcao;
    std::cout << "Choose an option:" << std::endl;
    std::cout << "1 - Analyze an image" << std::endl;
    std::cout << "0 - Exit" << std::endl;
    std::cin >> opcao;

    if (opcao == 1) {
        menuPrincipalImagem();
    } else if (opcao == 0) {
        return;
    } else {
        std::cout << "Invalid option." << std::endl;
    }
}

// Main function
int main() {
    menuPrincipal();
    return 0;
}
