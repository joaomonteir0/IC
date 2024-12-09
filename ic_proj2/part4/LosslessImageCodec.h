#ifndef LOSSLESS_IMAGE_CODEC_H
#define LOSSLESS_IMAGE_CODEC_H

#include <opencv2/opencv.hpp>
#include <string>

class LosslessImageCodec {
public:
    LosslessImageCodec(int golombM);
    void encode(const cv::Mat& image, const std::string& outputFile);
    cv::Mat decode(const std::string& inputFile);

private:
    int m; // Golomb parameter
    int predict(int a, int b, int c);
};

#endif // LOSSLESS_IMAGE_CODEC_H
