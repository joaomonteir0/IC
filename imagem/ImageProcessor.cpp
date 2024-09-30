#include <opencv2/opencv.hpp>
#include <iostream>

class ImageProcessor {
private:
    cv::Mat img;  // Variable to store the image

public:
    // Function to load the image
    bool loadImage(const std::string &imagePath) {
        img = cv::imread(imagePath);  // Load the image using OpenCV
        if (img.empty()) {            // Check if the image was loaded correctly
            std::cerr << "Erro: Imagem vazia." << std::endl;
            return false;
        }
        return true;
    }

    // Function to display the image
    void displayImage(const std::string &windowName) {
        cv::imshow(windowName, img);  // Display the image in the window
        cv::waitKey(0);               // Wait for a key press
    }

    // Function to display RGB channels
    void displayChannels() {
        if (img.empty()) {
            std::cerr << "Imagem não carregada." << std::endl;
            return;
        }

        std::vector<cv::Mat> channels;
        cv::split(img, channels);  // Split the image into channels

        cv::imshow("Canal Azul", channels[0]);  // Blue channel
        cv::imshow("Canal Verde", channels[1]); // Green channel
        cv::imshow("Canal Vermelho", channels[2]); // Red channel

        cv::Mat grayscale;
        cv::cvtColor(img, grayscale, cv::COLOR_BGR2GRAY);  // Convert to grayscale

        cv::imshow("Imagem em Escala de Cinza", grayscale);  // Display the grayscale image
        cv::waitKey(0); 

        cv::waitKey(0);  // Wait for a key press
    }
};
