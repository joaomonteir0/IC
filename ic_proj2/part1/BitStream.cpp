#include "BitStream.h"

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
    bool bit = (buffer >> (7 - bitPos)) & 1;
    bitPos = (bitPos + 1) % 8;
    return bit;
}

void BitStream::writeBits(const std::vector<int>& bits) {
    for (int bit : bits) {
        writeBit(bit);
    }
}

std::vector<int> BitStream::readBits(size_t n) {
    std::vector<int> bits;
    for (size_t i = 0; i < n; ++i) {
        bits.push_back(readBit());
    }
    return bits;
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

bool BitStream::hasMoreBits() {
    return file.peek() != EOF;
}

void BitStream::close() {
    if (mode & std::ios::out) {
        flushBuffer();
    }
    file.close();
}
