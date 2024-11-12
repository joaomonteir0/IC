#include "Golomb.h"
#include <cmath>

Golomb::Golomb(int m, bool useInterleaving) : m(m), useInterleaving(useInterleaving) {}

void Golomb::encode(int number, BitStream &bitStream) {
    if (useInterleaving) {
        number = interleave(number);
    } else {
        if (number < 0) {
            bitStream.writeBit(1);
            number = -number;
        } else {
            bitStream.writeBit(0);
        }
    }

    int q = number / m;
    int r = number % m;

    for (int i = 0; i < q; ++i) {
        bitStream.writeBit(1);
    }
    bitStream.writeBit(0);

    int b = std::ceil(std::log2(m));
    int threshold = (1 << b) - m;
    if (r < threshold) {
        bitStream.writeBits(r, b - 1);
    } else {
        r += threshold;
        bitStream.writeBits(r, b);
    }
}

int Golomb::decode(BitStream &bitStream) {
    int q = 0;
    while (bitStream.readBit()) {
        ++q;
    }

    int b = std::ceil(std::log2(m));
    int threshold = (1 << b) - m;
    int r = bitStream.readBits(b - 1);
    if (r >= threshold) {
        r = (r << 1) | bitStream.readBit();
        r -= threshold;
    }

    int number = q * m + r;

    if (!useInterleaving) {
        if (bitStream.readBit()) {
            number = -number;
        }
    } else {
        number = deinterleave(number);
    }

    return number;
}

int Golomb::interleave(int number) {
    return (number >= 0) ? 2 * number : -2 * number - 1;
}

int Golomb::deinterleave(int number) {
    return (number % 2 == 0) ? number / 2 : -(number / 2) - 1;
}
