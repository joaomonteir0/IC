#ifndef GOLOMB_H
#define GOLOMB_H

#include "BitStream.h"

class Golomb {
public:
    explicit Golomb(int m);
    void encode(BitStream& bitStream, int value);
    int decode(BitStream& bitStream);

private:
    int m;
    int calcUnary(int value);
};

#endif
