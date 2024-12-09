CXX = g++
CXXFLAGS = -Wall -std=c++17
LIBS = -lsndfile
OBJS_ENCODER = BitStream.o Golomb.o AudioEncoder.o
OBJS_DECODER = BitStream.o Golomb.o AudioDecoder.o

all: AudioEncoder AudioDecoder

AudioEncoder: $(OBJS_ENCODER)
	$(CXX) $(CXXFLAGS) -o AudioEncoder $(OBJS_ENCODER) $(LIBS)

AudioDecoder: $(OBJS_DECODER)
	$(CXX) $(CXXFLAGS) -o AudioDecoder $(OBJS_DECODER) $(LIBS)

BitStream.o: ../part1/BitStream.cpp ../part1/BitStream.h
	$(CXX) $(CXXFLAGS) -c ../part1/BitStream.cpp -o BitStream.o

Golomb.o: ../part2/Golomb.cpp ../part2/Golomb.h
	$(CXX) $(CXXFLAGS) -c ../part2/Golomb.cpp -o Golomb.o

AudioEncoder.o: AudioEncoder.cpp
	$(CXX) $(CXXFLAGS) -c AudioEncoder.cpp -o AudioEncoder.o

AudioDecoder.o: AudioDecoder.cpp
	$(CXX) $(CXXFLAGS) -c AudioDecoder.cpp -o AudioDecoder.o

clean:
	rm -f *.o AudioEncoder AudioDecoder
