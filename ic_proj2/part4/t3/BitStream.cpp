#include "BitStream.h"
#include <iostream>

BitStream::BitStream() = default;

BitStream::~BitStream() {
    if (fs.is_open()) {
        fs.close();
    }
}

void BitStream::openFile(const std::string &filename, std::ios::openmode mode) {
    fs.open(filename, mode);
    if (!fs.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
}

void BitStream::writeBit(int bit) {
    buffer = (buffer << 1) | (bit & 1);
    bufSize++;
    if (bufSize == 8) {
        fs.write(&buffer, 1);
        buffer = 0x00;
        bufSize = 0;
    }
}

void BitStream::flushBuffer() {
    if (bufSize > 0) {
        buffer <<= (8 - bufSize); // Align remaining bits to the left
        fs.write(&buffer, 1);    // Write the remaining bits to the file
        std::cout << "Flushed buffer with " << bufSize << " bits. Final padded byte: "
                  << static_cast<int>(buffer & 0xFF) << std::endl; // Debug output
        buffer = 0x00;
        bufSize = 0;
    } else {
        std::cout << "No leftover bits to flush. Buffer is already aligned." << std::endl;
    }
}


int BitStream::readBit() {
    if (bufSize == 0) {
        fs.read(&buffer, 1);
        if (fs.eof()) {
            return -1;
        }
        bufSize = 8;
    }
    int bit = (buffer >> (bufSize - 1)) & 1;
    bufSize--;
    return bit;
}

void BitStream::writeBits(uint64_t value, int n) {
    for (int i = n - 1; i >= 0; i--) {
        writeBit((value >> i) & 1);
    }
    std::cout << "Wrote " << n << " bits: " << value
              << " (Buffer state: " << static_cast<int>(buffer & 0xFF)
              << ", bufSize: " << bufSize << ")" << std::endl; // Debug output
}

uint64_t BitStream::readBits(int n) {
    uint64_t value = 0;
    for (int i = 0; i < n; i++) {
        int bit = readBit();
        if (bit == -1) {
            throw std::runtime_error("Unexpected end of file while reading bits.");
        }
        value = (value << 1) | bit;
    }
    std::cout << "Read " << n << " bits: " << value << std::endl; 
    return value;
}

bool BitStream::isEndOfFile() {
    return fs.eof();
}

void BitStream::alignToByte() {
    if (bufSize > 0) {
        std::cout << "Aligning to byte boundary. Ignoring " << bufSize << " leftover bits." << std::endl;
        buffer = 0x00; // Clear the buffer
        bufSize = 0;   // Reset the buffer size
    }
}

