#include "BitStream.h"
#include <iostream>
#include <fstream>
#include <chrono>

void encode(const std::string& inputFilename, const std::string& outputFilename) {
    std::ifstream inputFile(inputFilename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed to open input file");
    }

    BitStream bitStream(outputFilename, std::ios::out | std::ios::binary);

    char bitChar;
    while (inputFile >> bitChar) {
        if (bitChar == '0') {
            bitStream.writeBit(0);
        } else if (bitChar == '1') {
            bitStream.writeBit(1);
        } else {
            throw std::runtime_error("Invalid character in input file");
        }
    }

    inputFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: encoder <input text file> <output binary file>" << std::endl;
        return 1;
    }

    try {
        auto start = std::chrono::high_resolution_clock::now();

        encode(argv[1], argv[2]);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "Encoding completed in " << duration.count() << " seconds." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
