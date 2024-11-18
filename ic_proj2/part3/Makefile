CXX = g++
CXXFLAGS = -Wall -std=c++17
OBJS = BitStream.o Golomb.o AudioEncoder.o

all: AudioEncoder

AudioEncoder: $(OBJS)
	$(CXX) $(CXXFLAGS) -o AudioEncoder $(OBJS)

BitStream.o: BitStream.cpp BitStream.h
	$(CXX) $(CXXFLAGS) -c BitStream.cpp

Golomb.o: Golomb.cpp Golomb.h
	$(CXX) $(CXXFLAGS) -c Golomb.cpp

AudioEncoder.o: AudioEncoder.cpp
	$(CXX) $(CXXFLAGS) -c AudioEncoder.cpp

clean:
	rm -f $(OBJS) AudioEncoder
