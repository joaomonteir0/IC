#include "VideoCoder.h"
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <algorithm>

VideoCoder::VideoCoder(int bs, bool lossy, int qLevel, int tBitrate)
    : blockSize(bs), isLossy(lossy), quantizationLevel(qLevel), targetBitrate(tBitrate) {}

void VideoCoder::encodeVideo(const std::string& inputPath, const std::string& outputPath, int iFrameInterval, int blockSize, int searchRange) {
    cv::VideoCapture cap(inputPath);
    if (!cap.isOpened()) throw std::runtime_error("Cannot open input video");

    VideoHeader header = {
        static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH)),
        static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT)),
        blockSize,
        iFrameInterval,
        searchRange,
        cap.get(cv::CAP_PROP_FPS),
        static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT)),
        isLossy,
        quantizationLevel,
        targetBitrate
    };

    BitStream bs(outputPath, BitStream::Write);
    bs.writeBits(header.width, 16);
    bs.writeBits(header.height, 16);
    bs.writeBits(header.blockSize, 8);
    bs.writeBits(header.iFrameInterval, 8);
    bs.writeBits(header.searchRange, 8);
    bs.writeBits(static_cast<int>(header.fps), 16);
    bs.writeBits(header.totalFrames, 32);
    bs.writeBit(header.isLossy);
    if (header.isLossy) {
        bs.writeBits(header.quantizationLevel, 8);
        bs.writeBits(header.targetBitrate, 32);
    }

    cv::Mat frame, prevFrame;
    int frameCount = 0;

    while (cap.read(frame)) {
        bool isIFrame = (frameCount % iFrameInterval == 0);
        bs.writeBit(isIFrame);

        if (isIFrame) {
            // Encode I-frame (Lossless for simplicity)
            for (int y = 0; y < frame.rows; ++y) {
                for (int x = 0; x < frame.cols; ++x) {
                    cv::Vec3b pixel = frame.at<cv::Vec3b>(y, x);
                    for (int c = 0; c < 3; ++c) {
                        bs.writeBits(pixel[c], 8);
                    }
                }
            }
            frame.copyTo(prevFrame);
        } else {
            // Encode P-frame
            encodePFrame(bs, frame, prevFrame, header);
            frame.copyTo(prevFrame);
        }

        ++frameCount;
    }

    cap.release();
}

void VideoCoder::decodeVideo(const std::string& inputPath, const std::string& outputPath) {
    BitStream bs(inputPath, BitStream::Read);

    VideoHeader header;
    header.width = bs.readBits(16);
    header.height = bs.readBits(16);
    header.blockSize = bs.readBits(8);
    header.iFrameInterval = bs.readBits(8);
    header.searchRange = bs.readBits(8);
    header.fps = bs.readBits(16);
    header.totalFrames = bs.readBits(32);
    header.isLossy = bs.readBit();
    if (header.isLossy) {
        header.quantizationLevel = bs.readBits(8);
        header.targetBitrate = bs.readBits(32);
    }

    cv::VideoWriter writer(outputPath, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), header.fps, cv::Size(header.width, header.height));

    cv::Mat frame(header.height, header.width, CV_8UC3);
    cv::Mat prevFrame(header.height, header.width, CV_8UC3);

    for (int frameCount = 0; frameCount < header.totalFrames; ++frameCount) {
        bool isIFrame = bs.readBit();

        if (isIFrame) {
            for (int y = 0; y < frame.rows; ++y) {
                for (int x = 0; x < frame.cols; ++x) {
                    cv::Vec3b& pixel = frame.at<cv::Vec3b>(y, x);
                    for (int c = 0; c < 3; ++c) {
                        pixel[c] = bs.readBits(8);
                    }
                }
            }
            frame.copyTo(prevFrame);
        } else {
            decodePFrame(bs, frame, prevFrame, header);
            frame.copyTo(prevFrame);
        }

        writer.write(frame);
    }

    writer.release();
}

void VideoCoder::encodePFrame(BitStream& bs, const cv::Mat& currentFrame, const cv::Mat& referenceFrame, const VideoHeader& header) {
    for (int y = 0; y < currentFrame.rows; y += header.blockSize) {
        for (int x = 0; x < currentFrame.cols; x += header.blockSize) {
            cv::Point2i motionVector = estimateMotion(currentFrame, referenceFrame, x, y, header.blockSize, header.searchRange);
            bs.writeBits(motionVector.x + 128, 8);
            bs.writeBits(motionVector.y + 128, 8);

            cv::Rect refBlock(x + motionVector.x, y + motionVector.y, header.blockSize, header.blockSize);
            cv::Rect currBlock(x, y, header.blockSize, header.blockSize);

            cv::Mat residual = calculateResidual(currentFrame(currBlock), referenceFrame(refBlock));
            for (int i = 0; i < residual.rows; ++i) {
                for (int j = 0; j < residual.cols; ++j) {
                    bs.writeBits(residual.at<short>(i, j) + 128, 8);
                }
            }
        }
    }
}

void VideoCoder::decodePFrame(BitStream& bs, cv::Mat& currentFrame, const cv::Mat& referenceFrame, const VideoHeader& header) {
    for (int y = 0; y < currentFrame.rows; y += header.blockSize) {
        for (int x = 0; x < currentFrame.cols; x += header.blockSize) {
            int mvX = bs.readBits(8) - 128;
            int mvY = bs.readBits(8) - 128;

            cv::Rect refBlock(x + mvX, y + mvY, header.blockSize, header.blockSize);
            cv::Rect currBlock(x, y, header.blockSize, header.blockSize);

            cv::Mat residual(header.blockSize, header.blockSize, CV_16SC3);
            for (int i = 0; i < residual.rows; ++i) {
                for (int j = 0; j < residual.cols; ++j) {
                    residual.at<short>(i, j) = bs.readBits(8) - 128;
                }
            }

            cv::Mat reconstructed = reconstructBlock(referenceFrame(refBlock), residual);
            reconstructed.copyTo(currentFrame(currBlock));
        }
    }
}

cv::Point2i VideoCoder::estimateMotion(const cv::Mat& currentFrame, const cv::Mat& referenceFrame, int blockX, int blockY, int blockSize, int searchRange) {
    // Simplified motion estimation
    return cv::Point2i(0, 0);
}

cv::Mat VideoCoder::calculateResidual(const cv::Mat& current, const cv::Mat& predicted) {
    cv::Mat residual;
    cv::subtract(current, predicted, residual);
    return residual;
}

cv::Mat VideoCoder::reconstructBlock(const cv::Mat& predicted, const cv::Mat& residual) {
    cv::Mat reconstructed;
    cv::add(predicted, residual, reconstructed);
    return reconstructed;
}
