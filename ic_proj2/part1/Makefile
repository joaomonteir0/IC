CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRC_FILES = BitStream.cpp encoder.cpp decoder.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)

all: encoder decoder

encoder: encoder.o BitStream.o
	$(CXX) $(CXXFLAGS) -o $@ $^

decoder: decoder.o BitStream.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o encoder decoder *.bin decoded.txt

run: all
	./encoder input.txt output.bin
	./decoder output.bin decoded.txt
