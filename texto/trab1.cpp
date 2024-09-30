#include <fstream>
#include <iostream>
#include <cctype>     // Para funções de conversão de case
#include <algorithm>  // Para a função remove_if
#include <map>        // Para armazenar a contagem de frequências de caracteres

using namespace std;

// Função para normalizar o texto
void normalizeText(char* data, streamsize size) {
    for (streamsize i = 0; i < size; ++i) {
        // Converter cada caractere para minúsculo
        data[i] = tolower(data[i]);
    }

    // Remover pontuação
    auto isPunctuation = [](char ch) {
        return ispunct(static_cast<unsigned char>(ch));
    };

    // Usando std::remove_if para mover pontuações para o final
    char* end = remove_if(data, data + size, isPunctuation);

    // Redefinir o terminador nulo no final do texto modificado
    *end = '\0';
}

// Função para contar a frequência dos caracteres
void countCharacterFrequencies(const char* data, map<char, int>& freqMap) {
    for (const char* ptr = data; *ptr != '\0'; ++ptr) {
        // Ignorar espaços
        if (*ptr != ' ') {
            freqMap[*ptr]++; // Incrementar a contagem do caractere
        }
    }
}

//Função para contar a frequência de palavras
void countWordFrequencies(const char* data, map<string, int>& freqMap) {
    string word;
    for (const char* ptr = data; *ptr != '\0'; ++ptr) {
        if (*ptr == ' ') {
            if (!word.empty()) {
                freqMap[word]++;
                word.clear();
            }
        } else {
            word += *ptr;
        }
    }

    if (!word.empty()) {
        freqMap[word]++;
    }
}

int main() {
    ifstream infile;
    infile.open("teste.txt", ios::binary);

    if (infile.fail()) {
        cout << "Error opening the file" << endl;
        return 1;
    }

    infile.seekg(0, ios::end); // Ir para o fim do arquivo
    streamsize size = infile.tellg();
    infile.seekg(0, ios::beg); // Voltar ao início do arquivo

    // Alocar memória para armazenar o conteúdo do arquivo
    char* data = new char[size + 1];  // +1 para o terminador nulo
    infile.read(data, size);

    //Conteúdo do arquivo
    cout << "Size of file: " << size << " bytes" << endl;
    cout << "\nConteúdo do arquivo:" << endl;
    cout.write(data, size);
    cout << endl;
    
    // Adicionar o terminador nulo ao final da string
    data[size] = '\0';

    // Normalizar o texto (converter para minúsculas e remover pontuação)
    normalizeText(data, size);

    // Imprimir o texto normalizado
    cout << "\nTexto normalizado:" << endl;
    cout << data << endl;

    // Contar frequências de caracteres
    map<char, int> frequencyMap;
    countCharacterFrequencies(data, frequencyMap);

    // Imprimir a frequência de caracteres
    cout << "\nFrequências de caracteres:" << endl;
    for (const auto& pair : frequencyMap) {
        cout << "'" << pair.first << "' : " << pair.second << endl;
    }

    // Contar frequências de palavras
    map<string, int> wordFrequencyMap;
    countWordFrequencies(data, wordFrequencyMap);

    // Imprimir a frequência de palavras
    cout << "\nFrequências de palavras:" << endl;
    for (const auto& pair : wordFrequencyMap) {
        cout << pair.first << " : " << pair.second << endl;
    }

    // Liberação de memória
    delete[] data;
    infile.close();

    return 0;
}
