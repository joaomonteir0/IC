#include "BitStream.h"
#include <iostream>
#include <fstream>
#include <chrono>

void decode(const std::string& inputFilename, const std::string& outputFilename) {
    BitStream bitStream(inputFilename, std::ios::in | std::ios::binary);

    std::ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Failed to open output file");
    }

    try {
        while (true) {
            bool bit = bitStream.readBit();
            outputFile << (bit ? '1' : '0');
        }
    } catch (const std::ios_base::failure& e) {
        // End of file reached
    }

    outputFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: decoder <input binary file> <output text file>" << std::endl;
        return 1;
    }

    try {
        auto start = std::chrono::high_resolution_clock::now();

        decode(argv[1], argv[2]);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "Decoding completed in " << duration.count() << " seconds." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
