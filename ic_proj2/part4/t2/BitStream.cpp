#include "BitStream.h"
#include <iostream>
#include <stdexcept>

BitStream::BitStream(const std::string& filename, Mode mode) : buffer(0), bitPos(0), mode(mode) {
    if (mode == WRITE)
        file.open(filename, std::ios::binary | std::ios::out);
    else
        file.open(filename, std::ios::binary | std::ios::in);

    if (!file.is_open()) throw std::ios_base::failure("Failed to open file");
}

BitStream::~BitStream() {
    if (mode == WRITE && bitPos > 0) flushBuffer();
    file.close();
}

void BitStream::writeBits(uint32_t value, int numBits) {
    for (int i = numBits - 1; i >= 0; --i) {
        buffer = (buffer << 1) | ((value >> i) & 1);
        bitPos++;
        if (bitPos == 8) flushBuffer();
    }
}

uint32_t BitStream::readBits(int numBits) {
    uint32_t result = 0;
    for (int i = 0; i < numBits; ++i) {
        if (bitPos == 0) {
            file.read(reinterpret_cast<char*>(&buffer), 1);
            if (file.eof()) throw std::ios_base::failure("End of file reached");
            bitPos = 8;
        }
        result = (result << 1) | ((buffer >> (bitPos - 1)) & 1);
        bitPos--;
    }
    return result;
}

void BitStream::flushBuffer() {
    if (bitPos > 0) {
        buffer <<= (8 - bitPos);
        file.write(reinterpret_cast<char*>(&buffer), 1);
        buffer = 0;
        bitPos = 0;
    }
}

void BitStream::close() { file.close(); }

bool BitStream::eof() const { return file.eof(); }
