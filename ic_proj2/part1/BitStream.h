#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <cstdint>

class BitStream {
public:
    enum Mode { Write, Read };

    BitStream(const std::string& filename, Mode mode);
    ~BitStream();

    void writeBit(bool bit);
    bool readBit();

    void writeBits(uint64_t value, int n);
    uint64_t readBits(int n);

    bool hasMoreBits();

    void close();

private:
    std::fstream file;
    Mode mode;
    uint8_t buffer;
    int bitPos;

    void flush(); // Helper function for writing
};

#endif // BITSTREAM_H
