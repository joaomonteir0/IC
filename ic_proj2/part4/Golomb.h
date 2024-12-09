#ifndef GOLOMB_H
#define GOLOMB_H

#include "BitStream.h"

class Golomb {
public:
    Golomb(int m, bool useInterleaving = false);

    void encode(int number, BitStream &bitStream);
    int decode(BitStream &bitStream);

private:
    int m;
    bool useInterleaving;

    int interleave(int number);
    int uninterleave(int number);
};

#endif // GOLOMB_H
