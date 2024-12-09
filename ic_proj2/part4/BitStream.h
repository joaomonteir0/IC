#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdint>

class BitStream {
public:
    enum Mode { Write, Read };

    BitStream(const std::string& filename, Mode mode);
    ~BitStream();

    void writeBit(bool bit);
    bool readBit();

    void writeBits(uint32_t value, int bitCount);
    uint32_t readBits(int bitCount);

private:
    std::fstream file;
    Mode mode;
    uint8_t buffer;
    int bitPos;

    void flush();
};

#endif // BITSTREAM_H
