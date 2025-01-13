CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

SOURCES = BitStream.cpp Golomb.cpp VideoCoder.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

TARGETS = encode

all: $(TARGETS)

encode: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o encode $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o $(TARGETS)
