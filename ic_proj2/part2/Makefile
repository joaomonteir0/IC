CXX = g++
CXXFLAGS = -Wall -std=c++17

# Caminho para os ficheiros na Part 1
BITSTREAM_DIR = ../part1

# Programas e objetos
TARGETS = GolombEncoder GolombDecoder testGolomb
OBJECTS = GolombEncoder.o GolombDecoder.o testGolomb.o

all: $(TARGETS)

GolombEncoder: GolombEncoder.o
	$(CXX) $(CXXFLAGS) -o GolombEncoder GolombEncoder.o -lsndfile

GolombDecoder: GolombDecoder.o
	$(CXX) $(CXXFLAGS) -o GolombDecoder GolombDecoder.o -lsndfile

testGolomb: testGolomb.o
	$(CXX) $(CXXFLAGS) -o testGolomb testGolomb.o

GolombEncoder.o: GolombEncoder.cpp $(BITSTREAM_DIR)/BitStream.h $(BITSTREAM_DIR)/BitStream.h
	$(CXX) $(CXXFLAGS) -c GolombEncoder.cpp -o GolombEncoder.o -I$(BITSTREAM_DIR)

GolombDecoder.o: GolombDecoder.cpp $(BITSTREAM_DIR)/BitStream.h $(BITSTREAM_DIR)/BitStream.h
	$(CXX) $(CXXFLAGS) -c GolombDecoder.cpp -o GolombDecoder.o -I$(BITSTREAM_DIR)

testGolomb.o: testGolomb.cpp $(BITSTREAM_DIR)/BitStream.h $(BITSTREAM_DIR)/BitStream.h
	$(CXX) $(CXXFLAGS) -c testGolomb.cpp -o testGolomb.o -I$(BITSTREAM_DIR)

clean:
	rm -f $(TARGETS) *.o *.bin output.wav
