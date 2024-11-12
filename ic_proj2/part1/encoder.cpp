#include "BitStream.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

using namespace std;

void encode(const string &inputFilename, const string &outputFilename) {
    ifstream inputFile(inputFilename);
    if (!inputFile.is_open()) {
        throw runtime_error("Failed to open input file");
    }

    BitStream outputBitStream(outputFilename, ios::out | ios::binary);

    char bitChar;
    int bitCount = 0;

    while (inputFile >> bitChar) {
        if (bitChar == '0') {
            outputBitStream.writeBit(0);
        } else if (bitChar == '1') {
            outputBitStream.writeBit(1);
        } else {
            throw runtime_error("Invalid character in input file");
        }
        bitCount++;
    }

    while (bitCount % 8 != 0) {
        outputBitStream.writeBit(0);
        bitCount++;
    }

    inputFile.close();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: encoder <input text file> <output binary file>" << endl;
        return 1;
    }

    try {
        auto start = chrono::high_resolution_clock::now();

        encode(argv[1], argv[2]);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        cout << "Encoding completed in " << duration.count() << " seconds." << endl;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
