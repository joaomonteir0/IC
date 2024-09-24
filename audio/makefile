CXX = g++

CXXFLAGS = -Wall -std=c++17

SRC = audiodata_manipulation.cpp

TARGET = audiodata_manipulation

LIBS = -lsfml-audio -lsfml-system

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
