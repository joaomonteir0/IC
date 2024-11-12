CXX = g++
CXXFLAGS = -std=c++11 -Wall

PART1_DIR = ../part1
SRC_FILES = Golomb.cpp testGolomb.cpp $(PART1_DIR)/BitStream.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)

all: testGolomb

testGolomb: testGolomb.o Golomb.o BitStream.o
	$(CXX) $(CXXFLAGS) -o $@ $^

testGolomb.o: testGolomb.cpp
	$(CXX) $(CXXFLAGS) -I$(PART1_DIR) -c -o $@ $<

Golomb.o: Golomb.cpp
	$(CXX) $(CXXFLAGS) -I$(PART1_DIR) -c -o $@ $<

BitStream.o: $(PART1_DIR)/BitStream.cpp
	$(CXX) $(CXXFLAGS) -I$(PART1_DIR) -c -o $@ $<

clean:
	rm -f *.o testGolomb output.bin decoded.txt

run: all
	./testGolomb input.txt output.bin decoded.txt
