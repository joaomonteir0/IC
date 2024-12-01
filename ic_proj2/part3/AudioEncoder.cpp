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

    std::vector<short> samples(FRAMES_BUFFER_SIZE * inputFile.channels());
    size_t framesRead;

    while ((framesRead = inputFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
        for (size_t i = 0; i < framesRead * inputFile.channels(); ++i) {
            golomb.encode(samples[i], outputBitStream);
        }
    }

    outputBitStream.close();
    return 0;
}
