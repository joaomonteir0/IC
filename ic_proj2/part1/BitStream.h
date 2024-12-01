#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <cstdint>

class BitStream {
public:
    BitStream(const std::string& filename, std::ios::openmode mode);
    ~BitStream();

    void writeBit(bool bit);
    bool readBit();

    void writeBits(const std::vector<int>& bits);
    std::vector<int> readBits(size_t n);

    bool hasMoreBits();

    void close();

private:
    std::fstream file;
    uint8_t buffer;
    int bitPos;
    std::ios::openmode mode;

    void flushBuffer();
    void fillBuffer();
};

#endif
