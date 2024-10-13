#include <fstream>
#include <iostream>
#include <cctype>     // Para funções de conversão de case
#include <algorithm>  // Para a função remove_if
#include <map>        // Para armazenar a contagem de frequências de caracteres
#include <locale>
#include <codecvt>    // Para conversão de UTF-8
#include <regex>      // Para expressões regulares
#include <cmath>      // Para funções matemáticas
#include <SFML/Graphics.hpp> // Para a exibição de gráficos
#include "histogram.h"

using namespace std;

// Função para normalizar o texto
void normalizeText(wstring& data) {
    for (wchar_t& ch : data) {
        // Converter cada caractere para minúsculo
        ch = towlower(ch);
    }

    // Remover pontuação
    auto isPunctuation = [](wchar_t ch) {
        return iswpunct(ch);
    };

    // Usando std::remove_if para mover pontuações para o final
    data.erase(remove_if(data.begin(), data.end(), isPunctuation), data.end());
}

// Função para contar a frequência dos caracteres
void countCharacterFrequencies(const wstring& data, map<wchar_t, int>& freqMap) {
    for (const wchar_t& ch : data) {
        // Ignorar espaços
        if (ch != L' ') {
            freqMap[ch]++;
        }
    }
}

// Função para contar a frequência de palavras
void countWordFrequencies(const wstring& data, map<wstring, int>& freqMap) {
    wstring word;
    for (const wchar_t& ch : data) {
        if (ch == L' ') {
            if (!word.empty()) {
                freqMap[word]++;
                word.clear();
            }
        } else {
            word += ch;
        }
    }

    if (!word.empty()) {
        freqMap[word]++;
    }
}

// Função para remover tags XML
void removeXMLTags(wstring& data) {
    wregex xmlTagPattern(L"<[^>]*>");  // Padrão regex para encontrar tags XML
    data = regex_replace(data, xmlTagPattern, L""); // Substituir as tags por uma string vazia
}

// Função para calcular a entropia H(X)
double calculateEntropy(const map<wchar_t, int>& freqMap, int totalChars) {
    double entropy = 0.0;

    for (const auto& pair : freqMap) {
        double probability = static_cast<double>(pair.second) / totalChars;
        if (probability > 0) {  // Evitar log2(0)
            entropy -= probability * log2(probability);
        }
    }

    return entropy;
}

double calculateWordEntropy(const std::map<std::wstring, int>& freqMap, int totalWords) {
    double entropy = 0.0;
    for (const auto& pair : freqMap) {
        double probability = static_cast<double>(pair.second) / totalWords;
        if (probability > 0) {
            entropy -= probability * log(probability);
        }
    }
    return entropy;
}

// Função para exibir o menu e pegar a opção do usuário
int menu() {
    int option;
    wcout << L"\nEscolha uma opção de processamento de texto:" << endl;
    wcout << L"1. Converter para minúsculas" << endl;
    wcout << L"2. Remover pontuação" << endl;
    wcout << L"3. Remover tags XML" << endl; // Nova opção para remover tags XML
    wcout << L"4. Contar frequências de caracteres" << endl;
    wcout << L"5. Contar frequências de palavras" << endl;
    wcout << L"6. Exibir o texto" << endl;
    wcout << L"7. Exibir histogramas" << endl; // Mudou para plural
    wcout << L"0. Sair" << endl;
    wcout << L"Opção: ";
    wcin >> option;
    return option;
}

int main() {
    // Configurar o locale para UTF-8
    locale::global(locale(""));

    // Configurar a leitura UTF-8
    wifstream infile("teste.txt");
    infile.imbue(locale(infile.getloc(), new codecvt_utf8<wchar_t>));

    if (infile.fail()) {
        wcerr << L"Erro ao abrir o arquivo" << endl;
        return 1;
    }

    wstring data((istreambuf_iterator<wchar_t>(infile)), istreambuf_iterator<wchar_t>());
    infile.close();

    if (data.empty()) {
        wcerr << L"O arquivo está vazio." << endl;
        return 1;
    }

    int option = -1;
    map<wchar_t, int> frequencyMap;
    map<wstring, int> wordFrequencyMap; // Mapa para a frequência de palavras
    int totalChars = data.length(); // Total de caracteres

    double entropy = 0.0;
    int totalWords = 0;

    while (option != 0) {
        option = menu();

        switch (option) {
            case 1:
                normalizeText(data);
                wcout << L"\nTexto convertido para minúsculas:" << endl;
                wcout << data << endl;
                break;
            case 2:
                data.erase(remove_if(data.begin(), data.end(), iswpunct), data.end());
                wcout << L"\nTexto sem pontuação:" << endl;
                wcout << data << endl;
                break;
            case 3:
                removeXMLTags(data);
                wcout << L"\nTexto sem tags XML:" << endl;
                wcout << data << endl;
                break;
            case 4:
                frequencyMap.clear();
                countCharacterFrequencies(data, frequencyMap);
                wcout << L"\nFrequências de caracteres:" << endl;
                for (const auto& pair : frequencyMap) {
                    wcout << pair.first << L": " << pair.second << endl;
                }
                entropy = calculateEntropy(frequencyMap, totalChars); // Atualiza o valor de 'entropy'
                wcout << L"Entropia H(X): " << entropy << endl;
                break;
            case 5:
                wordFrequencyMap.clear();
                countWordFrequencies(data, wordFrequencyMap);
                wcout << L"\nFrequências de palavras:" << endl;
                for (const auto& pair : wordFrequencyMap) {
                    wcout << pair.first << L": " << pair.second << endl;
                }

                // Calcular e exibir a entropia para palavras
                for (const auto& pair : wordFrequencyMap) {
                    totalWords += pair.second;
                }
                entropy = calculateWordEntropy(wordFrequencyMap, totalWords); // Chama a nova função
                wcout << L"Entropia H(X) para palavras: " << entropy << endl; // Exibe a entropia
                break;
            case 6:
                wcout << L"\nTexto:" << endl;
                wcout << data << endl;
                break;
            case 7:
                displayCharacterHistogram(frequencyMap);
                displayWordHistogram(wordFrequencyMap);
                break;
            case 0:
                wcout << L"Saindo..." << endl;
                break;
            default:
                wcout << L"Opção inválida. Tente novamente." << endl;
                break;
        }
    }

    return 0;
}