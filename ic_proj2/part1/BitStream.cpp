#include "BitStream.h"
#include <vector> // Adicionado para resolver o erro de std::vector

BitStream::BitStream(const std::string& filename, std::ios::openmode mode) : buffer(0), bitPos(0), mode(mode) {
    file.open(filename, mode);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
}

BitStream::~BitStream() {
    if (mode & std::ios::out) {
        flushBuffer();
    }
    file.close();
}

void BitStream::writeBit(bool bit) {
    buffer = (buffer << 1) | bit;
    bitPos++;
    if (bitPos == 8) {
        flushBuffer();
    }
}

bool BitStream::readBit() {
    if (bitPos == 0) {
        fillBuffer();
    }
    if (file.eof()) {
        throw std::ios_base::failure("End of file reached");
    }
    bool bit = (buffer >> (7 - bitPos)) & 1;
    bitPos = (bitPos + 1) % 8;
    return bit;
}

void BitStream::writeBits(uint64_t value, int n) {
    for (int i = n - 1; i >= 0; --i) {
        writeBit((value >> i) & 1);
    }
}

uint64_t BitStream::readBits(int n) {
    uint64_t value = 0;
    for (int i = 0; i < n; ++i) {
        value = (value << 1) | readBit();
    }
    return value;
}

bool BitStream::hasMoreBits() {
    return file.peek() != EOF;
}

void BitStream::flushBuffer() {
    if (bitPos > 0) {
        buffer <<= (8 - bitPos);
        file.put(buffer);
        buffer = 0;
        bitPos = 0;
    }
}

void BitStream::fillBuffer() {
    if (!file.get(reinterpret_cast<char&>(buffer))) {
        throw std::ios_base::failure("End of file reached");
    }
    bitPos = 0;
}

void BitStream::close() {
    if (mode & std::ios::out) {
        flushBuffer();
    }
    file.close();
}
