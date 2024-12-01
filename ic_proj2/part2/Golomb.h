#ifndef GOLOMB_H
#define GOLOMB_H

#include "../part1/BitStream.h"
#include <string>
#include <vector>

class Golomb {
public:
    Golomb(int m, bool useInterleaving = false); // Adicione o segundo parâmetro com valor padrão
    void encode(int number, BitStream& bitStream);
    int decode(BitStream& bitStream);

private:
    int m;
    bool useInterleaving;
};

#endif
