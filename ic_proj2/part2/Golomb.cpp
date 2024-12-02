#include "Golomb.h"

Golomb::Golomb(int m) : m(m) {}

void Golomb::encode(int number, BitStream& bitStream) {
    bool isNegative = (number < 0);
    number = std::abs(number);

    // Calcula o quociente (q) e o resto (r)
    int q = number / m;
    int r = number % m;

    // Codifica o quociente em formato unário
    for (int i = 0; i < q; ++i) {
        bitStream.writeBit(1);
    }
    bitStream.writeBit(0);

    // Codifica o resto (r) em binário
    int b = std::ceil(std::log2(m));
    int threshold = (1 << b) - m;

    if (r < threshold) {
        bitStream.writeBits(r, b - 1);
    } else {
        r += threshold;
        bitStream.writeBits(r, b);
    }

    // Codifica o sinal
    bitStream.writeBit(isNegative);
}

int Golomb::decode(BitStream& bitStream) {
    // Decodifica o quociente (q) no formato unário
    int q = 0;
    while (bitStream.readBit()) {
        ++q;
    }

    // Decodifica o resto (r) em binário
    int b = std::ceil(std::log2(m));
    int threshold = (1 << b) - m;
    int r = bitStream.readBits(b - 1);

    if (r >= threshold) {
        r = (r << 1) | bitStream.readBit();
        r -= threshold;
    }

    // Reconstrói o número
    int number = q * m + r;

    // Verifica o sinal
    if (bitStream.readBit()) {
        number = -number;
    }

    return number;
}
