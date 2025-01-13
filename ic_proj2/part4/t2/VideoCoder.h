#ifndef VIDEOCODER_H
#define VIDEOCODER_H

#include <string>
#include <opencv2/opencv.hpp>
#include "BitStream.h"

struct VideoHeader {
    int width;              // frame width
    int height;             // frame height
    int blockSize;          // block size for encoding
    int iFrameInterval;     // Interval for intra frames
    int searchRange;        // Search range for motion estimation
    double fps;             // Frames per second
    int totalFrames;        // Total number of frames
    bool isLossy;           // Whether the compression is lossy
    int quantizationLevel;  // Quantization level for lossy compression
    int targetBitrate;      // Target bitrate for lossy compression
};


class VideoCoder {
public:
    void encodeVideo(const std::string& inputPath, const std::string& outputPath, int iFrameInterval, int blockSize, int searchRange);
    void decodeVideo(const std::string& inputPath, const std::string& outputPath);

private:
    void writeVideoHeader(BitStream& bs, const VideoHeader& header);
    VideoHeader readVideoHeader(BitStream& bs);

    void encodeIFrame(BitStream& bs, const cv::Mat& frame, const VideoHeader& header);
    void encodePFrame(BitStream& bs, const cv::Mat& frame, const cv::Mat& prevFrame, const VideoHeader& header);

    void decodeIFrame(BitStream& bs, cv::Mat& frame, const VideoHeader& header);
    void decodePFrame(BitStream& bs, cv::Mat& frame, const cv::Mat& prevFrame, const VideoHeader& header);
};

#endif // VIDEOCODER_H
