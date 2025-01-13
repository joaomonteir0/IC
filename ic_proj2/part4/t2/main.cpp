#include "VideoCoder.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <encode/decode> <input_file> <output_file> <parameter>" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string input = argv[2];
    std::string output = argv[3];
    int parameter = std::stoi(argv[4]);

    VideoCoder coder;

    try {
        if (mode == "encode") {
            coder.encodeVideo(input, output, parameter, 16, 16); // Example parameters: block size = 16, search range = 16
        } else if (mode == "decode") {
            coder.decodeVideo(input, output);
        } else {
            std::cerr << "Invalid mode: " << mode << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
