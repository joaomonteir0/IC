#include "LosslessImageCodec.h"
#include "Golomb.h"
#include "BitStream.h"
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

LosslessImageCodec::LosslessImageCodec(int golombM) : m(golombM) {}

int LosslessImageCodec::predict(int a, int b, int c) {
    // Use A (left pixel) predictor for simplicity
    return a;
}

void LosslessImageCodec::encode(const cv::Mat& image, const std::string& outputFile) {
    BitStream bitStream(outputFile, BitStream::Write);
    Golomb golomb(m);

    // Write image metadata
    bitStream.writeBits(image.rows, 16);
    bitStream.writeBits(image.cols, 16);
    bitStream.writeBits(image.type(), 8);

    // Predict and encode residuals
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            int pixel = image.at<uchar>(i, j);
            int a = (j > 0) ? image.at<uchar>(i, j - 1) : 0; // Left neighbor
            int predicted = predict(a, 0, 0);
            int residual = pixel - predicted;

            // Encode residual
            golomb.encode(residual, bitStream);
        }
    }
}

cv::Mat LosslessImageCodec::decode(const std::string& inputFile) {
    BitStream bitStream(inputFile, BitStream::Read);
    Golomb golomb(m);

    // Read image metadata
    int rows = bitStream.readBits(16);
    int cols = bitStream.readBits(16);
    int type = bitStream.readBits(8);

    // Prepare decoded image
    cv::Mat decodedImage(rows, cols, type);

    // Decode residuals and reconstruct image
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int a = (j > 0) ? decodedImage.at<uchar>(i, j - 1) : 0; // Left neighbor
            int predicted = predict(a, 0, 0);
            int residual = golomb.decode(bitStream);

            // Reconstruct pixel
            int pixel = residual + predicted;
            decodedImage.at<uchar>(i, j) = pixel;
        }
    }
    return decodedImage;
}
