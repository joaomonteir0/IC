#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "VideoCoder.h"

void encode(const std::string& inputY4M, const std::string& outputBin, int predictorType, int golombM) {
    std::ifstream input(inputY4M, std::ios::binary);
    if (!input.is_open()) {
        std::cerr << "Error: Could not open input file: " << inputY4M << std::endl;
        return;
    }

    std::string header;
    std::getline(input, header);
    if (header.find("YUV4MPEG2") == std::string::npos) {
        std::cerr << "Error: Invalid Y4M file format." << std::endl;
        return;
    }

    int width = 0, height = 0;
    for (size_t i = 0; i < header.size(); ++i) {
        if (header[i] == 'W') width = std::stoi(header.substr(i + 1));
        if (header[i] == 'H') height = std::stoi(header.substr(i + 1));
    }

    VideoCoder coder(golombM, width, height, predictorType);
    BitStream bitStream(outputBin, BitStream::WRITE);

    // Write header information
    bitStream.writeBits(width, 16);
    bitStream.writeBits(height, 16);
    bitStream.writeBits(golombM, 8);
    bitStream.writeBits(predictorType, 8);

    std::vector<std::vector<int>> frame(height, std::vector<int>(width));
    std::string frameHeader;
    while (std::getline(input, frameHeader)) {
        if (frameHeader.find("FRAME") == std::string::npos) continue;

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                frame[i][j] = input.get();
            }
        }

        coder.encodeFrame(frame, bitStream);
    }

    input.close();
    bitStream.close();

    std::cout << "Encoding completed: " << outputBin << std::endl;
}

void decode(const std::string& inputBin, const std::string& outputMp4, int predictorType, int golombM) {
    BitStream bitStream(inputBin, BitStream::READ);

    // Read header information
    int width = bitStream.readBits(16);
    int height = bitStream.readBits(16);
    int readGolombM = bitStream.readBits(8);
    int readPredictorType = bitStream.readBits(8);

    VideoCoder coder(readGolombM, width, height, readPredictorType);

    std::ofstream outputYUV("temp_output.yuv", std::ios::binary);
    if (!outputYUV.is_open()) {
        std::cerr << "Error: Could not open temporary output file." << std::endl;
        return;
    }

    std::vector<std::vector<int>> frame;
    try {
        while (!bitStream.eof()) {
            frame = coder.decodeFrame(bitStream);

            // Write the frame to the YUV file
            for (const auto& row : frame) {
                for (int pixel : row) {
                    outputYUV.put(static_cast<unsigned char>(pixel));
                }
            }
        }
    } catch (const std::ios_base::failure& e) {
        if (!bitStream.eof()) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    outputYUV.close();
    bitStream.close();

    // Convert YUV to MP4 using FFmpeg
    std::string command = "ffmpeg -y -f rawvideo -pix_fmt yuv420p -s " +
                          std::to_string(width) + "x" + std::to_string(height) +
                          " -i temp_output.yuv " + outputMp4;
    system(command.c_str());

    std::remove("temp_output.yuv");
    std::cout << "Decoding completed: " << outputMp4 << std::endl;
}


int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage:\n"
                  << "  Encode: ./encode input.y4m output.bin predictor_type\n"
                  << "  Decode: ./encode input.bin output.mp4 predictor_type\n";
        return 1;
    }

    std::string input = argv[1];
    std::string output = argv[2];
    int predictorType = std::stoi(argv[3]);
    int golombM = 4;  // Default Golomb parameter

    if (input.find(".y4m") != std::string::npos && output.find(".bin") != std::string::npos) {
        encode(input, output, predictorType, golombM);
    } else if (input.find(".bin") != std::string::npos && output.find(".mp4") != std::string::npos) {
        decode(input, output, predictorType, golombM);
    } else {
        std::cerr << "Invalid input/output file types." << std::endl;
        return 1;
    }

    return 0;
}
