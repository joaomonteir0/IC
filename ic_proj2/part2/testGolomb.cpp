#include "Golomb.h"
#include "../part1/BitStream.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: TesteGolomb <input text file> <output binary file> <decoded text file>" << endl;
        return 1;
    }

    string inputFilename = argv[1];
    string outputFilename = argv[2];
    string decodedFilename = argv[3];

    ifstream inputFile(inputFilename);
    if (!inputFile.is_open()) {
        cerr << "Failed to open input file" << endl;
        return 1;
    }

    BitStream outputBitStream(outputFilename, ios::out | ios::binary);
    int m = 10;
    Golomb golombEncoder(m);

    vector<int> numbers;    
    int number;
    while (inputFile >> number) {
        numbers.push_back(number);
        golombEncoder.encode(number, outputBitStream);
    }
    inputFile.close();

    outputBitStream.~BitStream();

    BitStream inputBitStream(outputFilename, ios::in | ios::binary);
    ofstream decodedFile(decodedFilename);
    if (!decodedFile.is_open()) {
        cerr << "Failed to open decoded file" << endl;
        return 1;
    }

    bool success = true;
    for (int originalNumber : numbers) {
        int decodedNumber = golombEncoder.decode(inputBitStream);
        decodedFile << decodedNumber << endl;
        if (decodedNumber != originalNumber) {
            success = false;
        }
    }
    decodedFile.close();

    if (success) {
        cout << "All numbers were successfully encoded and decoded." << endl;
    } else {
        cout << "There were mismatches in the encoding/decoding process." << endl;
    }

    return 0;
}
