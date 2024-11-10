#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdint>

class BitStream {
public:
    BitStream(const std::string& filename, std::ios::openmode mode);
    ~BitStream();

    void writeBit(bool bit);
    bool readBit();

    void writeBits(uint64_t value, int n);
    uint64_t readBits(int n);

    void writeString(const std::string& str);
    std::string readString(size_t length);

private:
    std::fstream file;
    uint8_t buffer;
    int bitPos;
    std::ios::openmode mode;

    void flushBuffer();
    void fillBuffer();
};

#endif
