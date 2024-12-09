#include <iostream>
#include "LosslessImageCodec.h"

int main() {
    LosslessImageCodec codec;

    std::string inputImage = "inputImage.txt";
    std::string encodedFile = "encoded.bin";
    std::string outputImage = "outputImage.txt";

    codec.encode(inputImage, encodedFile);
    std::cout << "Imagem codificada com sucesso!\n";

    codec.decode(encodedFile, outputImage);
    std::cout << "Imagem decodificada com sucesso!\n";

    return 0;
}
