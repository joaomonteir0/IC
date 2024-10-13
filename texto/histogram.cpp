#include "histogram.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

void saveScreenshot(sf::RenderWindow& window, const std::string& filename) {
    // Captura a tela da janela
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    texture.update(window);

    // Cria um arquivo de imagem
    if (!texture.copyToImage().saveToFile(filename)) {
        cerr << "Erro ao salvar a imagem: " << filename << endl;
    } else {
        cout << "Imagem salva como: " << filename << endl;
    }
}

void displayCharacterHistogram(const std::map<wchar_t, int>& histogram) {
    sf::RenderWindow window(sf::VideoMode(1800, 600), "Histograma de Caracteres");
    window.clear(sf::Color::White);

    int barWidth = 30;
    int maxCount = 0;

    // Encontrar o valor máximo
    for (const auto& pair : histogram) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
        }
    }

    int x = 50; // Posição inicial das barras
    for (const auto& pair : histogram) {
        float barHeight = (static_cast<float>(pair.second) / maxCount) * 400; // Normaliza a altura
        sf::RectangleShape bar(sf::Vector2f(barWidth, barHeight));
        bar.setFillColor(sf::Color(169, 169, 169)); // Cinza
        bar.setPosition(x, 500 - barHeight); // Posição da barra
        window.draw(bar);

        // Adiciona texto acima da barra para a contagem
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            // Lidar com erro ao carregar fonte
            return;
        }

        sf::Text countText;
        countText.setFont(font);
        countText.setString(std::to_string(pair.second)); // Contagem
        countText.setCharacterSize(15);
        countText.setFillColor(sf::Color::Black);
        countText.setPosition(x, 500 - barHeight - 20); // Posição do texto da contagem
        window.draw(countText);

        // Adiciona texto abaixo da barra para o caractere
        sf::Text charText;
        charText.setFont(font);
        charText.setString(std::wstring(1, pair.first)); // Caractere
        charText.setCharacterSize(15);
        charText.setFillColor(sf::Color::Black);
        charText.setPosition(x, 510); // Posição do texto do caractere
        window.draw(charText);

        x += barWidth + 10; // Espaçamento entre as barras
    }

    window.display();

    // Salvar a imagem do histograma de caracteres
    saveScreenshot(window, "character_histogram.png");

    // Espera até que a janela seja fechada
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}

void displayWordHistogram(const std::map<wstring, int>& freqMap) {
    int barWidth = 40; // Largura da barra
    int spacing = 5;  // Espaçamento entre as barras
    int numBars = freqMap.size(); // Número total de barras
    int windowWidth = 1800 + (numBars * (barWidth + spacing)); // Largura da janela baseada nas barras
    int windowHeight = 600; // Altura da janela

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Histograma de Palavras");
    window.clear(sf::Color::White);

    int maxCount = 0;

    // Encontrar o valor máximo
    for (const auto& pair : freqMap) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
        }
    }

    int x = 50; // Posição inicial das barras
    for (const auto& pair : freqMap) {
        float barHeight = (static_cast<float>(pair.second) / maxCount) * 400; // Normaliza a altura
        sf::RectangleShape bar(sf::Vector2f(barWidth, barHeight));
        bar.setFillColor(sf::Color(100, 149, 237)); // Azul
        bar.setPosition(x, 500 - barHeight); // Posição da barra
        window.draw(bar);

        // Adiciona texto acima da barra para a contagem
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            // Lidar com erro ao carregar fonte
            return;
        }

        sf::Text countText;
        countText.setFont(font);
        countText.setString(std::to_string(pair.second)); // Contagem
        countText.setCharacterSize(15);
        countText.setFillColor(sf::Color::Black);
        countText.setPosition(x + (barWidth / 2) - 10, 500 - barHeight - 20); // Centralizar o texto da contagem
        window.draw(countText);

        // Adiciona texto abaixo da barra para a palavra
        sf::Text wordText;
        wordText.setFont(font);
        wordText.setString(pair.first); // Palavra
        wordText.setCharacterSize(10);
        wordText.setFillColor(sf::Color::Black);

        // Posiciona a palavra centralizada em relação à barra
        float wordWidth = wordText.getGlobalBounds().width;
        wordText.setPosition(x + (barWidth / 2) - (wordWidth / 2), 510); // Centraliza a palavra
        window.draw(wordText);

        x += barWidth + spacing; // Espaçamento entre as barras
    }

    window.display();

    // Salvar a imagem do histograma de palavras
    saveScreenshot(window, "word_histogram.png");

    // Espera até que a janela seja fechada
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}