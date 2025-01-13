CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4`

SRCS = main.cpp VideoCoder.cpp Golomb.cpp BitStream.cpp
HDRS = VideoCoder.h Golomb.h BitStream.h Predictors.h
OBJS = $(SRCS:.cpp=.o)

TARGET = video_codec

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
