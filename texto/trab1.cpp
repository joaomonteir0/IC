#include <fstream>
#include <iostream>
#include <cctype>     // Para funções de conversão de case
#include <algorithm>  // Para a função remove_if
#include <map>        // Para armazenar a contagem de frequências de caracteres
#include <locale>
#include <codecvt>    // Para conversão de UTF-8

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

int main() {
    // Configurar o locale para UTF-8
    locale::global(locale(""));

    // Configurar a leitura UTF-8
    wifstream infile("teste.txt");
    infile.imbue(locale(infile.getloc(), new codecvt_utf8<wchar_t>));

    if (infile.fail()) {
        wcerr << L"Error opening the file" << endl;
        return 1;
    }

    wstring data((istreambuf_iterator<wchar_t>(infile)), istreambuf_iterator<wchar_t>());

    // Conteúdo do arquivo
    wcout << L"Conteúdo do arquivo:" << endl;
    wcout << data << endl;

    // Normalizar o texto (converter para minúsculas e remover pontuação)
    normalizeText(data);

    // Imprimir o texto normalizado
    wcout << L"\nTexto normalizado:" << endl;
    wcout << data << endl;

    // Contar frequências de caracteres
    map<wchar_t, int> frequencyMap;
    countCharacterFrequencies(data, frequencyMap);

    // Imprimir a frequência de caracteres
    wcout << L"\nFrequências de caracteres:" << endl;
    for (const auto& pair : frequencyMap) {
        wcout << L"'" << pair.first << L"' : " << pair.second << endl;
    }

    // Contar frequências de palavras
    map<wstring, int> wordFrequencyMap;
    countWordFrequencies(data, wordFrequencyMap);

    // Imprimir a frequência de palavras
    wcout << L"\nFrequências de palavras:" << endl;
    for (const auto& pair : wordFrequencyMap) {
        wcout << pair.first << L" : " << pair.second << endl;
    }

    infile.close();

    return 0;
}
