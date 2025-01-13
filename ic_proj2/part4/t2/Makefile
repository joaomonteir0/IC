CC = g++
CFLAGS = -Wall -g -std=c++17 `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4`

SRCS = main.cpp videoCoder.cpp BitStream.cpp Golomb.cpp Predictors.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = encode

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) *.bin *.mp4
