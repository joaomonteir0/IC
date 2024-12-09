#ifndef GOLOMB_H
#define GOLOMB_H

#include "../part1/BitStream.h"
#include <cmath>

class Golomb {
public:
    Golomb(int m);
    void encode(int number, BitStream& bitStream);
    int decode(BitStream& bitStream);

private:
    int m;
};

#endif
