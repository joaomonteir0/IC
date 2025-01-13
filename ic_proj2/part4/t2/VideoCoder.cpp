#include "VideoCoder.h"
#include "BitStream.h"
#include "Golomb.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

void VideoCoder::encodeVideo(const std::string& inputPath, const std::string& outputPath, int iFrameInterval, int blockSize, int searchRange) {
    cv::VideoCapture cap(inputPath, cv::CAP_FFMPEG);
    if (!cap.isOpened()) {
        throw std::runtime_error("Cannot open input video file");
    }

    VideoHeader header = {
        static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH)),  // width
        static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT)), // height
        blockSize,                                            // blockSize
        iFrameInterval,                                       // iFrameInterval
        searchRange,                                          // searchRange
        cap.get(cv::CAP_PROP_FPS),                            // fps
        static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT)),  // totalFrames
        false,                                                // isLossy
        0,                                                    // quantizationLevel
        0                                                     // targetBitrate
    };


    BitStream bs(outputPath, BitStream::Write);
    writeVideoHeader(bs, header);

    cv::Mat frame, prevFrame;
    int frameCount = 0;

    while (cap.read(frame)) {
        if (frame.empty()) break;

        bool isIFrame = (frameCount % iFrameInterval == 0);
        bs.writeBit(isIFrame);

        if (isIFrame) {
            encodeIFrame(bs, frame, header);
        } else {
            encodePFrame(bs, frame, prevFrame, header);
        }

        frame.copyTo(prevFrame);
        frameCount++;
    }
}

void VideoCoder::decodeVideo(const std::string& inputPath, const std::string& outputPath) {
    BitStream bs(inputPath, BitStream::Read);
    VideoHeader header = readVideoHeader(bs);

    cv::VideoWriter writer(outputPath, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), header.fps,
                           cv::Size(header.width, header.height));
    if (!writer.isOpened()) {
        throw std::runtime_error("Cannot open output video file");
    }

    cv::Mat frame, prevFrame;
    for (int frameCount = 0; frameCount < header.totalFrames; frameCount++) {
        bool isIFrame = bs.readBit();

        if (isIFrame) {
            decodeIFrame(bs, frame, header);
        } else {
            decodePFrame(bs, frame, prevFrame, header);
        }

        writer.write(frame);
        frame.copyTo(prevFrame);
    }
}

void VideoCoder::writeVideoHeader(BitStream& bs, const VideoHeader& header) {
    bs.writeBits(header.width, 16);
    bs.writeBits(header.height, 16);
    bs.writeBits(header.blockSize, 8);
    bs.writeBits(header.iFrameInterval, 8);
    bs.writeBits(header.searchRange, 8);
    bs.writeBits(static_cast<int>(header.fps), 16);
    bs.writeBits(header.totalFrames, 32);
}

VideoHeader VideoCoder::readVideoHeader(BitStream& bs) {
    VideoHeader header;
    header.width = bs.readBits(16);
    header.height = bs.readBits(16);
    header.blockSize = bs.readBits(8);
    header.iFrameInterval = bs.readBits(8);
    header.searchRange = bs.readBits(8);
    header.fps = bs.readBits(16);
    header.totalFrames = bs.readBits(32);
    return header;
}

void VideoCoder::encodeIFrame(BitStream& bs, const cv::Mat& frame, const VideoHeader& header) {
    for (int i = 0; i < frame.rows; i++) {
        for (int j = 0; j < frame.cols; j++) {
            for (int c = 0; c < frame.channels(); c++) {
                bs.writeBits(frame.at<cv::Vec3b>(i, j)[c], 8);
            }
        }
    }
}

void VideoCoder::encodePFrame(BitStream& bs, const cv::Mat& frame, const cv::Mat& prevFrame, const VideoHeader& header) {
    // Placeholder for actual motion estimation and residual encoding
    encodeIFrame(bs, frame, header);
}

void VideoCoder::decodeIFrame(BitStream& bs, cv::Mat& frame, const VideoHeader& header) {
    frame = cv::Mat(header.height, header.width, CV_8UC3);
    for (int i = 0; i < frame.rows; i++) {
        for (int j = 0; j < frame.cols; j++) {
            for (int c = 0; c < frame.channels(); c++) {
                frame.at<cv::Vec3b>(i, j)[c] = bs.readBits(8);
            }
        }
    }
}

void VideoCoder::decodePFrame(BitStream& bs, cv::Mat& frame, const cv::Mat& prevFrame, const VideoHeader& header) {
    // Placeholder for actual motion compensation and residual decoding
    decodeIFrame(bs, frame, header);
}
