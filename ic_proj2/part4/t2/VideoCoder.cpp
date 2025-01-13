#include "VideoCoder.h"
#include <iostream>

VideoCoder::VideoCoder(int golombM, int width, int height, int predictorType)
    : golombM(golombM), width(width), height(height), predictorType(predictorType), golomb(golombM) {}

int VideoCoder::predict(int left, int top, int topLeft) {
    switch (predictorType) {
    case 1: // JPEG-LS Predictor
        return left + top - topLeft;
    case 2: // Simple Left Predictor
        return left;
    case 3: // Simple Top Predictor
        return top;
    default: // No prediction
        return 0;
    }
}

void VideoCoder::encodeFrame(const std::vector<std::vector<int>>& frame, BitStream& bitStream) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int left = (j > 0) ? frame[i][j - 1] : 0;
            int top = (i > 0) ? frame[i - 1][j] : 0;
            int topLeft = (i > 0 && j > 0) ? frame[i - 1][j - 1] : 0;

            int predicted = predict(left, top, topLeft);
            int residual = frame[i][j] - predicted;

            golomb.encode(bitStream, residual);
        }
    }
}

std::vector<std::vector<int>> VideoCoder::decodeFrame(BitStream& bitStream) {
    std::vector<std::vector<int>> frame(height, std::vector<int>(width, 0));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int left = (j > 0) ? frame[i][j - 1] : 0;
            int top = (i > 0) ? frame[i - 1][j] : 0;
            int topLeft = (i > 0 && j > 0) ? frame[i - 1][j - 1] : 0;

            int predicted = predict(left, top, topLeft);
            int residual = golomb.decode(bitStream);

            frame[i][j] = predicted + residual;
        }
    }

    return frame;
}
