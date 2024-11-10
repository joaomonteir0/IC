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

void BitStream::writeString(const std::string& str) {
    for (char c : str) {
        writeBits(static_cast<uint8_t>(c), 8);
    }
}

std::string BitStream::readString(size_t length) {
    std::string str;
    for (size_t i = 0; i < length; ++i) {
        str.push_back(static_cast<char>(readBits(8)));
    }
    return str;
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
