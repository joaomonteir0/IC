CXX = g++

CXXFLAGS = -Wall -std=c++17

SRC = audiodata_manipulation.cpp

TARGET = audiodata_manipulation

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
