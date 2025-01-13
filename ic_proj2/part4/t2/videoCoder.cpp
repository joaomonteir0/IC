#include "videoCoder.h"
#include <opencv2/opencv.hpp>
#include <algorithm> // For std::clamp

VideoCoder::VideoCoder(const std::string &inputFile, const std::string &outputFile, bool mode)
    : input(inputFile), output(outputFile), encodeMode(mode) {}

void VideoCoder::encodeVideo() {
    cv::VideoCapture cap(input);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error: Unable to open input video file.");
    }

    int frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    BitStream bitStream;
    bitStream.openFile(output, std::ios::out | std::ios::binary);

    // Store dimensions in the bitstream
    bitStream.writeBits(frameWidth, 16);  // Use 16 bits for width
    bitStream.writeBits(frameHeight, 16); // Use 16 bits for height

    Golomb golomb(4);

    cv::Mat frame;
    while (cap.read(frame)) {
        for (int row = 0; row < frame.rows; ++row) {
            for (int col = 0; col < frame.cols; ++col) {
                for (int channel = 0; channel < 3; ++channel) {
                    int a = 0, b = 0, c = 0; // Replace with actual neighbors
                    int prediction = Predictors().predictJPEG_LS(a, b, c);

                    int residual = frame.at<cv::Vec3b>(row, col)[channel] - prediction;
                    golomb.encode(bitStream, residual);
                }
            }
        }
    }
    bitStream.flushBuffer();
}


void VideoCoder::decodeVideo() {
    BitStream bitStream;
    bitStream.openFile(input, std::ios::in | std::ios::binary);

    // Read dimensions from the bitstream
    int frameWidth = bitStream.readBits(16);
    int frameHeight = bitStream.readBits(16);

    Golomb golomb(4);

    cv::VideoWriter writer(output, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), 30, cv::Size(frameWidth, frameHeight), true);
    if (!writer.isOpened()) {
        throw std::runtime_error("Error: Unable to open output video file.");
    }

    cv::Mat frame(frameHeight, frameWidth, CV_8UC3);
    while (!bitStream.isEndOfFile()) {
        for (int row = 0; row < frame.rows; ++row) {
            for (int col = 0; col < frame.cols; ++col) {
                for (int channel = 0; channel < 3; ++channel) {
                    int a = 0, b = 0, c = 0; // Replace with actual neighbors
                    int prediction = Predictors().predictJPEG_LS(a, b, c);

                    int residual = golomb.decode(bitStream);
                    frame.at<cv::Vec3b>(row, col)[channel] = std::clamp(prediction + residual, 0, 255);
                }
            }
        }
        writer.write(frame);
    }
}
