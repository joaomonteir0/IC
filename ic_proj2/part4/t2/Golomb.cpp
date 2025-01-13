#include "Golomb.h"
#include <cmath>

Golomb::Golomb(int m) : m(m) {}

void Golomb::encode(BitStream& bitStream, int value) {
    int q = value / m;
    int r = value % m;

    // Unary encoding of q
    for (int i = 0; i < q; ++i) bitStream.writeBits(1, 1);
    bitStream.writeBits(0, 1);

    // Binary encoding of r
    int numBits = static_cast<int>(std::ceil(std::log2(m)));
    bitStream.writeBits(r, numBits);
}

int Golomb::decode(BitStream& bitStream) {
    int q = 0;

    // Decode unary part
    while (bitStream.readBits(1) == 1) ++q;

    // Decode binary part
    int numBits = static_cast<int>(std::ceil(std::log2(m)));
    int r = bitStream.readBits(numBits);

    return q * m + r;
}
