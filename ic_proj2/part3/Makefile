CXX = g++
CXXFLAGS = -Wall -std=c++17

# Diretórios das partes anteriores
PART1_DIR = ../part1
PART2_DIR = ../part2

# Programas alvo
TARGETS = AudioCodec GolombEncoder GolombDecoder

all: $(TARGETS)

# Compilar o codificador de áudio
audio_codec: AudioCodec.cpp $(PART1_DIR)/BitStream.h $(PART2_DIR)/Golomb.h $(PART2_DIR)/GolombEncoder.cpp $(PART2_DIR)/GolombDecoder.cpp
	$(CXX) $(CXXFLAGS) -o audio_codec AudioCodec.cpp $(PART2_DIR)/GolombEncoder.cpp $(PART2_DIR)/GolombDecoder.cpp -lsndfile -I$(PART1_DIR) -I$(PART2_DIR)

# Compilar o codificador Golomb
GolombEncoder: $(PART2_DIR)/GolombEncoder.cpp $(PART1_DIR)/BitStream.h $(PART2_DIR)/Golomb.h
	$(CXX) $(CXXFLAGS) -o GolombEncoder $(PART2_DIR)/GolombEncoder.cpp -lsndfile -I$(PART1_DIR) -I$(PART2_DIR)

# Compilar o descodificador Golomb
GolombDecoder: $(PART2_DIR)/GolombDecoder.cpp $(PART1_DIR)/BitStream.h $(PART2_DIR)/Golomb.h
	$(CXX) $(CXXFLAGS) -o GolombDecoder $(PART2_DIR)/GolombDecoder.cpp -lsndfile -I$(PART1_DIR) -I$(PART2_DIR)

# Limpar ficheiros gerados
clean:
	rm -f $(TARGETS) *.bin *.txt
