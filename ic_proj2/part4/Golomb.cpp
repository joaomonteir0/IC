#include "Golomb.h"
#include <cmath>

Golomb::Golomb(int m, bool useInterleaving) : m(m), useInterleaving(useInterleaving) {}

int Golomb::interleave(int number) {
    return (number >= 0) ? (number << 1) : ((-number << 1) - 1);
}

int Golomb::uninterleave(int number) {
    return (number & 1) ? -(number >> 1) - 1 : (number >> 1);
}

void Golomb::encode(int number, BitStream &bitStream) {
    if (useInterleaving) {
        number = interleave(number);
    }
    int q = number / m;
    int r = number % m;

    for (int i = 0; i < q; ++i) bitStream.writeBit(1);
    bitStream.writeBit(0);

    int b = std::ceil(std::log2(m));
    int threshold = (1 << b) - m;
    if (r < threshold) {
        bitStream.writeBits(r, b - 1);
    } else {
        bitStream.writeBits(r + threshold, b);
    }
}

int Golomb::decode(BitStream &bitStream) {
    int q = 0;
    while (bitStream.readBit()) ++q;

    int b = std::ceil(std::log2(m));
    int threshold = (1 << b) - m;
    int r = bitStream.readBits(b - 1);
    if (r >= threshold) {
        r = (r << 1) + bitStream.readBit() - threshold;
    }

    int number = q * m + r;
    return useInterleaving ? uninterleave(number) : number;
}
