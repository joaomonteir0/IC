#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <fstream>
#include <cstdint>

class BitStream {
public:
    enum Mode { READ, WRITE };

    BitStream(const std::string& filename, Mode mode);
    ~BitStream();

    void writeBits(uint32_t value, int numBits);
    uint32_t readBits(int numBits);

    void close();
    bool eof() const;

private:
    std::fstream file;
    uint8_t buffer;
    int bitPos;
    Mode mode;

    void flushBuffer();
};

#endif
