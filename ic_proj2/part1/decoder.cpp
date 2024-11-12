#include "BitStream.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

using namespace std;

void decode(const string &inputFilename, const string &outputFilename) {
    BitStream inputBitStream(inputFilename, ios::in | ios::binary);
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        throw runtime_error("Failed to open output file");
    }

    try {
        while (true) {
            bool bit = inputBitStream.readBit();
            outputFile << (bit ? '1' : '0');
        }
    } catch (const ios_base::failure& e) {
        // End of file reached
    }

    outputFile.close();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: decoder <input binary file> <output text file>" << endl;
        return 1;
    }

    try {
        auto start = chrono::high_resolution_clock::now();

        decode(argv[1], argv[2]);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        cout << "Decoding completed in " << duration.count() << " seconds." << endl;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
