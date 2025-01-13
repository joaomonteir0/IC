#ifndef VIDEOCODER_H
#define VIDEOCODER_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "BitStream.h"
#include "Golomb.h"

struct VideoHeader {
    int width;
    int height;
    int blockSize;
    int iFrameInterval;
    int searchRange;
    double fps;
    int totalFrames;
    bool isLossy;
    int quantizationLevel;
    int targetBitrate;
};

class VideoCoder {
public:
    VideoCoder(int blockSize = 16, bool isLossy = false, int quantizationLevel = 1, int targetBitrate = 0);
    void encodeVideo(const std::string& inputPath, const std::string& outputPath, int iFrameInterval, int blockSize, int searchRange);
    void decodeVideo(const std::string& inputPath, const std::string& outputPath);

private:
    int blockSize;
    bool isLossy;
    int quantizationLevel;
    int targetBitrate;

    cv::Point2i estimateMotion(const cv::Mat& currentFrame, const cv::Mat& referenceFrame, int blockX, int blockY, int blockSize, int searchRange);
    cv::Mat calculateResidual(const cv::Mat& current, const cv::Mat& predicted);
    cv::Mat reconstructBlock(const cv::Mat& predicted, const cv::Mat& residual);

    void encodePFrame(BitStream& bs, const cv::Mat& currentFrame, const cv::Mat& referenceFrame, const VideoHeader& header);
    void decodePFrame(BitStream& bs, cv::Mat& currentFrame, const cv::Mat& referenceFrame, const VideoHeader& header);
};

#endif // VIDEOCODER_H
