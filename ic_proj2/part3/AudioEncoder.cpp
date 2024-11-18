#include "BitStream.h"
#include "Golomb.h"
#include <iostream>
#include <fstream>
#include <cstdint>

void encodeAudio(const std::string &inputAudioFile, const std::string &outputFile, int m) {
    std::ifstream inputFile(inputAudioFile, std::ios::binary);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed to open input audio file");
    }

    BitStream outputBitStream(outputFile, std::ios::out | std::ios::binary);
    Golomb golombEncoder(m);

    int16_t previousSample = 0;
    int16_t currentSample;
    while (inputFile.read(reinterpret_cast<char *>(&currentSample), sizeof(currentSample))) {
        int residual = currentSample - previousSample;
        golombEncoder.encode(residual, outputBitStream);
        previousSample = currentSample;
    }

    inputFile.close();
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: AudioEncoder <input audio file> <output file> <golomb parameter m>" << std::endl;
        return 1;
    }

    try {
        std::string inputAudioFile = argv[1];
        std::string outputFile = argv[2];
        int m = std::stoi(argv[3]);
        encodeAudio(inputAudioFile, outputFile, m);
        std::cout << "Audio encoding completed successfully." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
