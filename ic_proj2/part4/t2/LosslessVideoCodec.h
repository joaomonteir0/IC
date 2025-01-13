#ifndef LOSSLESS_VIDEO_CODEC_H
#define LOSSLESS_VIDEO_CODEC_H

#include "BitStream.h"
#include "Golomb.h"
#include "Predictors.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class LosslessVideoCodec {
public:
    LosslessVideoCodec(int golombM);
    void encode(const std::string &inputVideo, const std::string &outputFile, Predictors::Standards predictorType);
    void decode(const std::string &inputFile, const std::string &outputVideo, Predictors::Standards predictorType);

private:
    int m;
    int mapToNonNegative(int n);
    int mapToSigned(int n);
    std::vector<std::vector<int>> calculateResiduals(const cv::Mat &image, Predictors::Standards standard);
    cv::Mat reconstructImage(const std::vector<std::vector<int>> &residuals, Predictors::Standards standard);
};

#endif
