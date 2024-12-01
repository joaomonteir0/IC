#include "Golomb.h"
#include <cmath>

Golomb::Golomb(int m, bool useInterleaving) : m(m), useInterleaving(useInterleaving) {}

void Golomb::encode(int number, BitStream& bitStream) {
    bool isNegative = (number < 0);
    number = std::abs(number);

    int q = number / m;
    int r = number % m;

    for (int i = 0; i < q; ++i) {
        bitStream.writeBit(1);
    }
    bitStream.writeBit(0);

    int b = std::ceil(std::log2(m));
    int threshold = (1 << b) - m;

    if (r < threshold) {
        bitStream.writeBits(std::vector<int>(b - 1, r));
    } else {
        r += threshold;
        bitStream.writeBits(std::vector<int>(b, r));
    }

    bitStream.writeBit(isNegative);
}

int Golomb::decode(BitStream& bitStream) {
    int q = 0;
    while (bitStream.readBit()) {
        ++q;
    }

    int b = std::ceil(std::log2(m));
    int threshold = (1 << b) - m;
    int r = 0;

    for (int i = 0; i < b - 1; ++i) {
        r = (r << 1) | bitStream.readBit();
    }

    if (r >= threshold) {
        r = (r << 1) | bitStream.readBit();
        r -= threshold;
    }

    int number = q * m + r;
    if (bitStream.readBit()) {
        number = -number;
    }

    return number;
}
