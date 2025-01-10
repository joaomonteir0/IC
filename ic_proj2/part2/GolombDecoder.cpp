#include "Golomb.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sndfile.hh>
#include "../part1/BitStream.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer para leitura de frames

// Função para prever o próximo valor com base nos 3 anteriores
inline int predict(int a, int b, int c) {
    return 3 * a - 3 * b + c;
}

// Lê e converte bits do cabeçalho
int readHeader(BitStream &bs, int size) {
    vector<int> bits = bs.readBits(size);
    int value = 0;
    for (size_t i = 0; i < bits.size(); i++)
        value += bits[i] * pow(2, bits.size() - i - 1);
    return value;
}

// Valida parâmetros recuperados do cabeçalho
void validateHeader(const string &param, int value) {
    if (value < 0) {
        cerr << "Erro: valor inválido para " << param << ": " << value << endl;
        exit(1);
    }
}

int main(int argc, char **argv) {
    // Verifica argumentos
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <arquivo de entrada> <arquivo de saída>\n";
        return 1;
    }

    // Inicia o temporizador
    clock_t start = clock();

    // Parâmetros iniciais
    int sampleRate = 44100;
    BitStream bs(argv[1], "r");

    // Leitura do cabeçalho
    int nChannels = readHeader(bs, 16);
    validateHeader("nChannels", nChannels);

    int padding = readHeader(bs, 16);
    validateHeader("padding", padding);

    int q = readHeader(bs, 16);
    validateHeader("quantization factor", q);

    int nFrames = readHeader(bs, 32);
    validateHeader("nFrames", nFrames);

    int blockSize = readHeader(bs, 16);
    validateHeader("blockSize", blockSize);

    int num_zeros = readHeader(bs, 16);
    validateHeader("num_zeros", num_zeros);

    int m_size = readHeader(bs, 16);
    validateHeader("m_size", m_size);

    // Configura o arquivo de saída
    SndfileHandle sfhOut(argv[2], SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, nChannels, sampleRate);

    // Lê os valores de M
    vector<int> m_vector;
    for (int i = 0; i < m_size; i++) {
        int m_i = readHeader(bs, 16);
        validateHeader("m_i", m_i);
        m_vector.push_back(m_i);
    }

    // Lê os bits codificados
    int total = bs.getFileSize() - (16 + 2 * m_size); // Corrige tamanho do cabeçalho
    long totalBits = total * 8;
    vector<int> encodedBits = bs.readBits(totalBits);

    // Validação do tamanho dos bits codificados
    if (encodedBits.empty()) {
        cerr << "Erro: nenhum dado codificado encontrado!\n";
        exit(1);
    }

    // Converte para string e remove zeros extras
    string encoded;
    for (auto bit : encodedBits) encoded += to_string(bit);
    encoded = encoded.substr(0, encoded.size() - num_zeros);

    // Decodifica os dados
    Golomb g;
    vector<int> decoded = (m_size == 1)
                              ? g.decode(encoded, m_vector[0])
                              : g.decodeMultiple(encoded, m_vector, blockSize);

    // Reconstrói os valores originais
    vector<short> samplesVector;

    auto reconstructSamples = [&](int start, int end) {
        for (int i = start; i < end; i++) {
            if (i >= 3) {
                int difference = decoded[i] + predict(samplesVector[i - 1], samplesVector[i - 2], samplesVector[i - 3]);
                samplesVector.push_back(difference);
            } else {
                samplesVector.push_back(decoded[i]);
            }
        }
    };

    if (nChannels < 2) {
        reconstructSamples(0, decoded.size());
    } else {
        reconstructSamples(0, nFrames);
        reconstructSamples(nFrames, decoded.size());

        // Mescla os dois canais
        vector<short> merged;
        for (int i = 0; i < nFrames; i++) {
            merged.push_back(samplesVector[i]);
            merged.push_back(samplesVector[nFrames + i]);
        }
        samplesVector = merged;
    }

    // Desquantiza os valores
    auto dequantize = [&](int shift) {
        for (auto &sample : samplesVector) {
            sample = (sample << shift) | 1;
        }
    };

    if (q != 0) {
        q == 1 ? dequantize(1) : dequantize(q);
    }

    // Remove preenchimento adicional
    if (padding > 0 && samplesVector.size() > (size_t)padding) {
        samplesVector.resize(samplesVector.size() - padding);
    } else if (padding > 0) {
        cerr << "Erro: padding maior que o tamanho dos dados!\n";
        exit(1);
    }

    // Cálculo do SNR
    double numerador = 0.0;
    double denominador = 0.0;

    for (size_t i = 0; i < samplesVector.size(); i++) {
        double original_sample = decoded[i];  // Original antes da codificação
        double reconstruído_sample = samplesVector[i];  // Após reconstrução
        numerador += original_sample * original_sample;
        denominador += pow(original_sample - reconstruído_sample, 2);
    }

    if (denominador == 0) {
        cout << "SNR: Infinito (sem ruído)" << endl;
    } else {
        double snr = 10 * log10(numerador / denominador);
        cout << "SNR: " << snr << " dB" << endl;
    }

    // Escreve no arquivo de saída
    sfhOut.write(samplesVector.data(), samplesVector.size());
    bs.close();

    // Exibe o tempo decorrido
    clock_t end = clock();
    double elapsed_secs = double(end - start) / CLOCKS_PER_SEC * 1000;
    cout << "Tempo decorrido: " << elapsed_secs << " ms" << endl;

    return 0;
}
