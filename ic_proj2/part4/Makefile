# Compiler name
CXX = g++

# Compilation flags
CXXFLAGS = -std=c++17 -Wall -O2

# Object files
OBJS = main.o LosslessImageCodec.o BitStream.o Golomb.o

# Executable name
TARGET = codec

# Default rule
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compiling individual object files
main.o: main.cpp LosslessImageCodec.h
	$(CXX) $(CXXFLAGS) -c main.cpp

LosslessImageCodec.o: LosslessImageCodec.cpp LosslessImageCodec.h BitStream.h Golomb.h
	$(CXX) $(CXXFLAGS) -c LosslessImageCodec.cpp

BitStream.o: BitStream.cpp BitStream.h
	$(CXX) $(CXXFLAGS) -c BitStream.cpp

Golomb.o: Golomb.cpp Golomb.h BitStream.h
	$(CXX) $(CXXFLAGS) -c Golomb.cpp

# Cleaning build files
clean:
	rm -f $(TARGET) $(OBJS)

# Running the program
run: $(TARGET)
	./$(TARGET)
