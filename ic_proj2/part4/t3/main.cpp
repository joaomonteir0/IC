#include "videoCoder.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 7) {
        std::cerr << "Usage: " << argv[0] << " <encode|decode> <input> <output> <iframeInterval> <blockSize> <searchRange>" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string input = argv[2];
    std::string output = argv[3];
    int iframeInterval = std::stoi(argv[4]);
    int blockSize = std::stoi(argv[5]);
    int searchRange = std::stoi(argv[6]);

    try {
        VideoCoder coder(input, output, mode == "encode", iframeInterval, blockSize, searchRange);
        if (mode == "encode") {
            coder.encodeVideo();
        } else if (mode == "decode") {
            coder.decodeVideo();
        } else {
            throw std::invalid_argument("Invalid mode. Use 'encode' or 'decode'.");
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
