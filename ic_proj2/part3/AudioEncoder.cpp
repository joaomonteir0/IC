#include "../part1/BitStream.h"
#include "../part2/Golomb.h"
#include <sndfile.hh>
#include <vector>
#include <iostream>

constexpr size_t FRAMES_BUFFER_SIZE = 65536;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: AudioEncoder <input audio file> <output file> <golomb parameter m>" << std::endl;
        return 1;
    }

    SndfileHandle inputFile(argv[1]);
    if (inputFile.error()) {
        std::cerr << "Error: invalid input file." << std::endl;
        return 1;
    }

    int m = std::stoi(argv[3]);
    BitStream outputBitStream(argv[2], std::ios::out | std::ios::binary);
    Golomb golomb(m);

    // Escreve o cabeçalho do arquivo WAV
    char header[44];
    inputFile.seek(0, SEEK_SET);
    inputFile.read(reinterpret_cast<short*>(header), 44 / sizeof(short));
    for (int i = 0; i < 44; ++i) {
        outputBitStream.writeBits(static_cast<uint8_t>(header[i]), 8);
    }

    // Lê amostras de áudio e codifica
    std::vector<short> samples(FRAMES_BUFFER_SIZE * inputFile.channels());
    size_t framesRead;
    int prevSample = 0; // Amostra anterior para calcular a diferença

    while ((framesRead = inputFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
        for (size_t i = 0; i < framesRead * inputFile.channels(); ++i) {
            int difference = samples[i] - prevSample; // Calcula a diferença
            golomb.encode(difference, outputBitStream);
            prevSample = samples[i]; // Atualiza a amostra anterior
        }
    }

    outputBitStream.close();
    std::cout << "Audio encoding completed successfully." << std::endl;
    return 0;
}
