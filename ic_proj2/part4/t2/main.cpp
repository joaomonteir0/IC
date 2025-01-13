#include <iostream>
#include "VideoCoder.h"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <encode/decode> <input> <output> <iFrameInterval>\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string input = argv[2];
    std::string output = argv[3];
    int iFrameInterval = std::stoi(argv[4]);

    try {
        VideoCoder coder;
        if (mode == "encode") {
            coder.encodeVideo(input, output, iFrameInterval, 16, 16);
        } else if (mode == "decode") {
            coder.decodeVideo(input, output);
        } else {
            std::cerr << "Invalid mode. Use 'encode' or 'decode'.\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
