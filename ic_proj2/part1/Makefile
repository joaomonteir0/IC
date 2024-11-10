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
	rm -f *.o encoder decoder *.bin output_*

run: all
	@echo "\nRunning for input_binary.txt..."
	./encoder input_binary.txt output_binary.bin
	./decoder output_binary.bin output_binary.txt
	@echo "Done. Check output_binary.txt for results."
