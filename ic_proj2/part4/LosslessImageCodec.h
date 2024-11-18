#ifndef LOSSLESS_IMAGE_CODEC_H
#define LOSSLESS_IMAGE_CODEC_H

#include "BitStream.h"
#include "Golomb.h"
#include <vector>
#include <string>

class LosslessImageCodec {
public:
    void encode(const std::string& inputImagePath, const std::string& outputBinaryPath);
    void decode(const std::string& inputBinaryPath, const std::string& outputImagePath);

private:
    int predictPixel(int left, int above, int aboveLeft);
    std::vector<std::vector<int>> loadImage(const std::string& path); // Placeholder
    void saveImage(const std::string& path, const std::vector<std::vector<int>>& image); // Placeholder
};

#endif // LOSSLESS_IMAGE_CODEC_H
