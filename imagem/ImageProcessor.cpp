#include <opencv2/opencv.hpp>
#include <iostream>

class ImageProcessor {
private:
    cv::Mat img;  // Variável privada para armazenar a imagem

public:
    // Função para carregar a imagem
    bool loadImage(const std::string &imagePath) {
        img = cv::imread(imagePath);  // Carregar a imagem usando OpenCV
        if (img.empty()) {            // Verificar se a imagem foi carregada corretamente
            std::cerr << "Erro: Imagem vazia." << std::endl;
            return false;
        }
        return true;
    }

    // Função para exibir a imagem
    void displayImage(const std::string &windowName) {
        cv::imshow(windowName, img);  // Exibir a imagem na janela
        cv::waitKey(0);               // Aguardar por uma tecla
    }
};
