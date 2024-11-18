# Nome do compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -std=c++17 -Wall -O2

# Arquivos fonte
SOURCES = main.cpp LosslessImageCodec.cpp BitStream.cpp Golomb.cpp

# Arquivos de cabeçalho
HEADERS = LosslessImageCodec.h BitStream.h Golomb.h

# Nome do executável
TARGET = codec

# Regras de compilação
all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Limpeza
clean:
	rm -f $(TARGET) *.o

# Regra para rodar o programa
run: $(TARGET)
	./$(TARGET)
