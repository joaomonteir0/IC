#include "../part1/BitStream.h"
#include "../part2/Golomb.h"
#include <sndfile.hh>
#include <vector>
#include <cmath>
#include <iostream>

constexpr size_t FRAMES_BUFFER_SIZE = 65536;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: AudioDecoder <input file> <output file> <golomb parameter m>" << std::endl;
        return 1;
    }

    BitStream inputBitStream(argv[1], std::ios::in | std::ios::binary);
    SndfileHandle outputFile(argv[2], SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 2, 44100);
    Golomb golomb(std::stoi(argv[3]));

    // Lê cabeçalho do arquivo codificado e escreve no arquivo decodificado
    std::vector<int> headerBits = inputBitStream.readBits(44 * 8);
    std::vector<uint8_t> headerBytes(44);
    for (size_t i = 0; i < headerBytes.size(); ++i) {
        for (int j = 0; j < 8; ++j) {
            headerBytes[i] = (headerBytes[i] << 1) | headerBits[i * 8 + j];
        }
    }
    outputFile.write(reinterpret_cast<short*>(headerBytes.data()), 44 / sizeof(short));

    // Decodifica diferenças e reconstrói amostras
    std::vector<short> samples;
    while (inputBitStream.hasMoreBits()) { // Usa o novo método para verificar EOF
        int difference = golomb.decode(inputBitStream);
        if (!samples.empty()) {
            difference += samples.back(); // Reconstrói a amostra
        }
        samples.push_back(static_cast<short>(difference));
    }

    // Escreve amostras decodificadas no arquivo de saída
    outputFile.write(samples.data(), samples.size());
    std::cout << "Audio decoding completed successfully." << std::endl;
    return 0;
}
