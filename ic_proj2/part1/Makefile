CXX = g++
CXXFLAGS = -Wall -std=c++17

TARGETS = testBitStream
OBJECTS = testBitStream.o

testBitStream: testBitStream.o
	$(CXX) $(CXXFLAGS) testBitStream.o -o testBitStream

testBitStream.o: testBitStream.cpp
	$(CXX) $(CXXFLAGS) -c testBitStream.cpp -o testBitStream.o

clean:
	rm -f $(OBJECTS) testBitStream test.bin
