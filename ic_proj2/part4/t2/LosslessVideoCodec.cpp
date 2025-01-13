#include "LosslessVideoCodec.h"
#include <cmath>
#include <algorithm>
#include <iostream>

LosslessVideoCodec::LosslessVideoCodec(int golombM) : m(golombM) {}

int LosslessVideoCodec::mapToNonNegative(int n) {
    return (n >= 0) ? 2 * n : (-2 * n - 1);
}

int LosslessVideoCodec::mapToSigned(int n) {
    return (n % 2 == 0) ? (n / 2) : (-(n + 1) / 2);
}

std::vector<std::vector<int>> LosslessVideoCodec::calculateResiduals(const cv::Mat &image, Predictors::Standards standard) {
    std::vector<std::vector<int>> residuals(image.channels());
    int rows = image.rows;
    int cols = image.cols;

    residuals[0].push_back(rows);
    residuals[0].push_back(cols);

    for (int c = 0; c < image.channels(); ++c) {
        residuals[c].reserve(rows * cols + 2);
        if (c > 0) {
            residuals[c].push_back(rows);
            residuals[c].push_back(cols);
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                uchar actual = image.at<cv::Vec3b>(i, j)[c];
                int left = (j > 0) ? image.at<cv::Vec3b>(i, j - 1)[c] : 0;
                int above = (i > 0) ? image.at<cv::Vec3b>(i - 1, j)[c] : 0;
                int diag = (i > 0 && j > 0) ? image.at<cv::Vec3b>(i - 1, j - 1)[c] : 0;

                int predicted = Predictors().predictors(standard, left, above, diag);
                int residual = static_cast<int>(actual) - predicted;

                residuals[c].push_back(mapToNonNegative(residual));
            }
        }
    }
    return residuals;
}

cv::Mat LosslessVideoCodec::reconstructImage(const std::vector<std::vector<int>> &residuals, Predictors::Standards standard) {
    int rows = residuals[0][0];
    int cols = residuals[0][1];
    int channels = residuals.size();

    cv::Mat reconstructed = cv::Mat::zeros(rows, cols, CV_8UC3);

    for (int c = 0; c < channels; ++c) {
        int idx = 2;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (idx >= residuals[c].size()) {
                    throw std::runtime_error("Residual index out of bounds in frame reconstruction.");
                }

                int left = (j > 0) ? reconstructed.at<cv::Vec3b>(i, j - 1)[c] : 0;
                int above = (i > 0) ? reconstructed.at<cv::Vec3b>(i - 1, j)[c] : 0;
                int diag = (i > 0 && j > 0) ? reconstructed.at<cv::Vec3b>(i - 1, j - 1)[c] : 0;

                int predicted = Predictors().predictors(standard, left, above, diag);
                int pixel = predicted + residuals[c][idx++];
                pixel = ((pixel % 256) + 256) % 256;
                reconstructed.at<cv::Vec3b>(i, j)[c] = static_cast<uchar>(pixel);
            }
        }
    }
    return reconstructed;
}

void LosslessVideoCodec::encode(const std::string &inputVideo, const std::string &outputFile, Predictors::Standards predictorType) {
    cv::VideoCapture cap(inputVideo);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error: Could not open input video.");
    }

    BitStream bitStream(outputFile, BitStream::Write);

    int frameCount = 0;
    cv::Mat frame;
    while (cap.read(frame)) {
        std::cout << "Processing frame " << frameCount + 1 << "..." << std::endl;

        if (frame.empty() || frame.rows <= 0 || frame.cols <= 0) {
            std::cerr << "Warning: Skipping invalid frame " << frameCount + 1 << "." << std::endl;
            continue;
        }

        auto residuals = calculateResiduals(frame, predictorType);

        bitStream.writeBits(frame.rows, 16);
        bitStream.writeBits(frame.cols, 16);

        for (int c = 0; c < residuals.size(); ++c) {
            Golomb golombCoder(m);
            for (size_t i = 2; i < residuals[c].size(); ++i) {
                golombCoder.encode(residuals[c][i], bitStream);
            }

            std::cout << "Frame " << frameCount + 1 << ", Channel " << c
                      << ", Residual size: " << residuals[c].size() << " (Expected: " << frame.rows * frame.cols + 2 << ")" << std::endl;
        }

        frameCount++;
    }

    bitStream.writeBits(frameCount, 32);
    cap.release();
    std::cout << "Encoding complete. Total frames: " << frameCount << std::endl;
}

void LosslessVideoCodec::decode(const std::string &inputFile, const std::string &outputVideo, Predictors::Standards predictorType) {
    BitStream bitStream(inputFile, BitStream::Read);

    int frameCount = bitStream.readBits(32);
    std::cout << "Decoding " << frameCount << " frames..." << std::endl;

    cv::VideoWriter writer(outputVideo, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, cv::Size(640, 480));
    if (!writer.isOpened()) {
        throw std::runtime_error("Error: Could not open output video.");
    }

    for (int frameIdx = 0; frameIdx < frameCount; frameIdx++) {
        std::cout << "Decoding frame " << frameIdx + 1 << "..." << std::endl;

        int rows = bitStream.readBits(16);
        int cols = bitStream.readBits(16);

        if (rows <= 0 || cols <= 0) {
            std::cerr << "Error: Invalid metadata for frame " << frameIdx + 1 << ". Rows: " << rows << ", Cols: " << cols << std::endl;
            return;
        }

        int channels = 3;
        std::vector<std::vector<int>> residuals(channels);
        for (int c = 0; c < channels; ++c) {
            residuals[c].push_back(rows);
            residuals[c].push_back(cols);

            Golomb golombCoder(m);
            for (int i = 0; i < rows * cols; ++i) {
                int encodedResidual = golombCoder.decode(bitStream);
                residuals[c].push_back(mapToSigned(encodedResidual));
            }
        }

        cv::Mat reconstructedFrame = reconstructImage(residuals, predictorType);
        writer.write(reconstructedFrame);
    }

    writer.release();
    std::cout << "Decoding complete. Video saved to " << outputVideo << std::endl;
}
