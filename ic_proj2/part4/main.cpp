#include "LosslessImageCodec.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <encode/decode> <input_file> <output_file>\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    LosslessImageCodec codec(4); // Initialize with Golomb parameter m = 4

    if (mode == "encode") {
        cv::Mat image = cv::imread(inputFile, cv::IMREAD_GRAYSCALE);
        if (image.empty()) {
            std::cerr << "Error: Could not open input file.\n";
            return 1;
        }

        codec.encode(image, outputFile);
        std::cout << "Image successfully encoded to " << outputFile << "\n";
    } else if (mode == "decode") {
        cv::Mat decodedImage = codec.decode(inputFile);

        cv::imwrite(outputFile, decodedImage);
        std::cout << "Image successfully decoded to " << outputFile << "\n";
    } else {
        std::cerr << "Invalid mode. Use 'encode' or 'decode'.\n";
        return 1;
    }

    return 0;
}
