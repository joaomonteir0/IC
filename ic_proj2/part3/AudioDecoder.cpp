#include "../part1/BitStream.h"
#include "../part2/Golomb.h"
#include <sndfile.hh>
#include <vector>
#include <iostream>

constexpr size_t FRAMES_BUFFER_SIZE = 65536;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: AudioDecoder <input file> <output file> <golomb parameter m>" << std::endl;
        return 1;
    }

    BitStream inputBitStream(argv[1], std::ios::in | std::ios::binary);
    int m = std::stoi(argv[3]);
    Golomb golomb(m);

    // Lê o cabeçalho do arquivo codificado
    char header[44];
    for (int i = 0; i < 44; ++i) {
        header[i] = static_cast<char>(inputBitStream.readBits(8));
    }

    // Configura o arquivo de saída
    SndfileHandle outputFile(argv[2], SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 2, 44100);
    if (outputFile.error()) {
        std::cerr << "Error: unable to open output file." << std::endl;
        return 1;
    }
    outputFile.write(reinterpret_cast<short*>(header), 44 / sizeof(short));

    // Decodifica as diferenças e reconstrói as amostras
    std::vector<short> samples;
    int prevSample = 0; // Amostra anterior para reconstruir
    while (inputBitStream.hasMoreBits()) {
        int difference = golomb.decode(inputBitStream); // Lê a diferença
        int sample = prevSample + difference;           // Reconstrói a amostra
        samples.push_back(static_cast<short>(sample));
        prevSample = sample;                            // Atualiza a amostra anterior
    }

    // Escreve as amostras no arquivo de saída
    outputFile.write(samples.data(), samples.size());
    std::cout << "Audio decoding completed successfully." << std::endl;
    return 0;
}
