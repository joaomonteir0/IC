#include "LosslessImageCodec.h"
#include <fstream>
#include <sstream>

// Load image from text file
std::vector<std::vector<int>> LosslessImageCodec::loadImage(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open image file");
    }

    std::vector<std::vector<int>> image;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        int value;
        while (iss >> value) {
            row.push_back(value);
        }
        image.push_back(row);
    }

    file.close();
    return image;
}

// Save image to text file
void LosslessImageCodec::saveImage(const std::string& path, const std::vector<std::vector<int>>& image) {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to save image file");
    }

    for (const auto& row : image) {
        for (int pixel : row) {
            file << pixel << " ";
        }
        file << "\n";
    }

    file.close();
}

// Predict pixel value
int LosslessImageCodec::predictPixel(int left, int above, int aboveLeft) {
    return (left + above) / 2;
}

// Encode image
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

// Decode image
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
