#include "LosslessImageCodec.h"

void LosslessImageCodec::encode(const std::string& inputImagePath, const std::string& outputBinaryPath) {
    BitStream bitStream(outputBinaryPath, BitStream::Mode::Write);
    Golomb golomb(4);

    auto image = loadImage(inputImagePath);
    int height = image.size();
    int width = image[0].size();

    bitStream.writeBits(width, 16);
    bitStream.writeBits(height, 16);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int predicted = (x == 0 || y == 0) ? 0 : predictPixel(image[y][x - 1], image[y - 1][x], image[y - 1][x - 1]);
            int residual = image[y][x] - predicted;
            golomb.encode(residual, bitStream);
        }
    }
}

void LosslessImageCodec::decode(const std::string& inputBinaryPath, const std::string& outputImagePath) {
    BitStream bitStream(inputBinaryPath, BitStream::Mode::Read);
    Golomb golomb(4);

    int width = bitStream.readBits(16);
    int height = bitStream.readBits(16);

    std::vector<std::vector<int>> image(height, std::vector<int>(width));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int predicted = (x == 0 || y == 0) ? 0 : predictPixel(image[y][x - 1], image[y - 1][x], image[y - 1][x - 1]);
            int residual = golomb.decode(bitStream);
            image[y][x] = predicted + residual;
        }
    }

    saveImage(outputImagePath, image);
}
