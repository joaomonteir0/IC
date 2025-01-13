#ifndef VIDEOCODER_H
#define VIDEOCODER_H

#include "Golomb.h"
#include "BitStream.h"
#include <vector>
#include <string>

class VideoCoder {
public:
    VideoCoder(int golombM, int width, int height, int predictorType);
    void encodeFrame(const std::vector<std::vector<int>>& frame, BitStream& bitStream);
    std::vector<std::vector<int>> decodeFrame(BitStream& bitStream);

private:
    int golombM;
    int width, height;
    int predictorType;
    Golomb golomb;

    int predict(int left, int top, int topLeft);
};

#endif
