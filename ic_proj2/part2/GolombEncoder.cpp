#include "Golomb.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>
#include <sndfile.hh>
#include "../part1/BitStream.h"

using namespace std;

// Tamanho máximo do buffer para frames
constexpr size_t FRAMES_BUFFER_SIZE = 65536;

// Função para prever o próximo valor com base em 3 valores anteriores
inline int predict(int a, int b, int c) {
    return 3 * a - 3 * b + c;
}

// Função para calcular 'm' com base em 'u'
inline int calc_m(int u) {
    if (u <= 0) return 1; // Evita divisão por zero ou log negativo
    return (int)-(1 / log((double)u / (1 + u)));
}

// Valida argumentos passados ao programa
bool validateArgs(int argc, char *argv[], bool &autoMode, bool &quantization, int &q) {
    if (argc < 4 || argc > 6) {
        cerr << "Uso: " << argv[0] << " <input file> <output file> <m | bs> [auto] [q]\n";
        return false;
    }

    // Modo automático ou quantização
    if (argc >= 5) {
        if (strcmp(argv[4], "auto") == 0) {
            autoMode = true;
        } else {
            q = atoi(argv[4]);
            quantization = true;
        }
    }

    if (argc == 6) {
        q = atoi(argv[5]);
        quantization = true;
    }

    if (q > 16 || q < 0) {
        cerr << "[q] deve estar entre 0 e 16\n";
        return false;
    }

    return true;
}

int main(int argc, char *argv[]) {
    // Variáveis de configuração
    bool autoMode = false, quantization = false;
    int q = 0;

    // Valida argumentos
    if (!validateArgs(argc, argv, autoMode, quantization, q)) return 1;

    // Abre o arquivo de entrada
    SndfileHandle sfhIn{argv[1]};
    if (sfhIn.error()) {
        cerr << "Erro: arquivo de entrada inválido\n";
        return 1;
    }

    // Valida formato do arquivo
    if ((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV ||
        (sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
        cerr << "Erro: formato do arquivo deve ser WAV PCM_16\n";
        return 1;
    }

    string output = argv[2];
    int m = atoi(argv[3]);
    int bs = m, og = m;

    // Inicia o temporizador
    clock_t start = clock();

    // Lê informações básicas
    size_t nFrames = sfhIn.frames();
    size_t nChannels = sfhIn.channels();
    vector<short> samples(nChannels * nFrames);
    sfhIn.readf(samples.data(), nFrames);

    // Calcula blocos e padding
    size_t nBlocks = static_cast<size_t>(ceil((double)nFrames / bs));
    samples.resize(nBlocks * bs * nChannels);
    int padding = samples.size() - nFrames * nChannels;

    // Divide canais, se necessário
    vector<short> left(samples.size() / 2), right(samples.size() / 2);
    if (nChannels > 1) {
        for (size_t i = 0; i < samples.size() / 2; i++) {
            left[i] = samples[i * nChannels];
            right[i] = samples[i * nChannels + 1];
        }
    }

    // Aplica quantização
    if (quantization) {
        for (auto &sample : samples) {
            sample >>= q; // Remove os bits menos significativos
        }
    }

    // Inicializa vetores
    vector<int> m_vector, valuesToBeEncoded;

    // Codifica os valores com predição
    auto encodeValues = [&](const vector<short> &samples) {
        for (size_t i = 0; i < samples.size(); i++) {
            int diff = (i >= 3) ? samples[i] - predict(samples[i - 1], samples[i - 2], samples[i - 3])
                                : samples[i];
            valuesToBeEncoded.push_back(diff);

            // Atualiza 'm' dinamicamente
            if (autoMode && i % bs == 0 && i != 0) {
                int sum = 0;
                for (size_t j = i - bs; j < i; j++) sum += abs(valuesToBeEncoded[j]);
                int u = round(sum / bs);
                m = calc_m(u);
                m = max(1, m);
                m_vector.push_back(m);
            }
        }
    };

    // Codifica canais
    if (nChannels < 2) {
        encodeValues(samples);
    } else {
        encodeValues(left);
        encodeValues(right);
    }

    // Gera string codificada
    string encodedString;
    Golomb g;
    int m_index = 0;
    for (size_t i = 0; i < valuesToBeEncoded.size(); i++) {
        if (autoMode && i % bs == 0 && i != 0) m_index++;
        encodedString += g.encode(valuesToBeEncoded[i], autoMode ? m_vector[m_index] : og);
    }

    // Escreve no arquivo de saída
    BitStream bitStream(output, "w");
    vector<int> bits, encodedBits;

    for (char c : encodedString) encodedBits.push_back(c - '0');
    int count_zeros = 0;

    while (encodedBits.size() % 8 != 0) {
        encodedBits.push_back(0);
        count_zeros++;
    }

    auto writeBits = [&](int value, int size) {
        for (int i = size - 1; i >= 0; i--) bits.push_back((value >> i) & 1);
    };

    writeBits(nChannels, 16);
    writeBits(padding, 16);
    writeBits(q, 16);
    writeBits(nFrames, 32);
    writeBits(bs, 16);
    writeBits(count_zeros, 16);
    writeBits(autoMode ? m_vector.size() : 1, 16);
    for (int m : m_vector) writeBits(m, 16);
    bits.insert(bits.end(), encodedBits.begin(), encodedBits.end());

    bitStream.writeBits(bits);
    bitStream.close();

    // Exibe tempo decorrido
    clock_t end = clock();
    cout << "Tempo de execução: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";

    return 0;
}
