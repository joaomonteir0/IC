#ifndef GOLOMB_H
#define GOLOMB_H

#include "../part1/BitStream.h"
#include <string>

class Golomb {
public:
    Golomb(int m, bool useInterleaving = false);
    void encode(int number, BitStream &bitStream);
    int decode(BitStream &bitStream);

private:
    int m;
    bool useInterleaving;

    int interleave(int number);
    int deinterleave(int number);
};

#endif
