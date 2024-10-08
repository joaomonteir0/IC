#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <map>
#include <cmath>
#include <filesystem>
#include <algorithm>

std::string chosenFile;

// Função para listar arquivos na pasta 'audio_files' e permitir que o usuário escolha um arquivo
void chooseAudioFile() {
    std::vector<std::string> files;
    std::cout << "Available audio files:\n";
    for (const auto& entry : std::filesystem::directory_iterator("audio_files")) {
        if (entry.path().extension() == ".wav") {
            files.push_back(entry.path().filename().string());
        }
    }

    // Ordenar os arquivos em ordem alfabética
    std::sort(files.begin(), files.end());

    // Exibir os arquivos ordenados
    for (size_t i = 0; i < files.size(); ++i) {
        std::cout << (i + 1) << ". " << files[i] << "\n";
    }

    int choice;
    std::cout << "Choose a file number: ";
    std::cin >> choice;

    if (choice < 1 || static_cast<std::vector<std::string>::size_type>(choice) > files.size()) {
        std::cerr << "Invalid choice. Exiting...\n";
        exit(1);
    }

    chosenFile = "audio_files/" + files[choice - 1];
    std::cout << "Chosen file: " << chosenFile << "\n";
}

// Função para carregar e exibir informações básicas do arquivo de áudio (T1)
void loadAudioFile() {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(chosenFile)) {
        std::cerr << "Error loading audio file: " << chosenFile << std::endl;
        return;
    }

    unsigned int sampleRate = buffer.getSampleRate();
    unsigned int channelCount = buffer.getChannelCount();
    sf::Time duration = buffer.getDuration();

    std::cout << "Audio file information:" << std::endl;
    std::cout << "Sample rate: " << sampleRate << " Hz" << std::endl;
    std::cout << "Number of channels: " << channelCount << std::endl;
    std::cout << "Duration: " << std::fixed << std::setprecision(2) << duration.asSeconds() << " seconds" << std::endl;

    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    std::cout << "First 10 raw audio samples: ";
    for (std::size_t i = 0; i < std::min(static_cast<std::size_t>(10), sampleCount); ++i) {
        std::cout << samples[i] << " ";
    }
    std::cout << std::endl;
}

// Função para visualizar a forma de onda do áudio (T2)
void visualizeWaveform() {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(chosenFile)) {
        std::cerr << "Error loading audio file: " << chosenFile << std::endl;
        return;
    }

    unsigned int sampleRate = buffer.getSampleRate();
    sf::Time duration = buffer.getDuration();

    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    sf::RenderWindow window(sf::VideoMode(1200, 600), "Waveform of Audio Sample");

    sf::VertexArray waveform(sf::LinesStrip, sampleCount);

    for (std::size_t i = 0; i < sampleCount; ++i) {
        float x = static_cast<float>(i) / sampleRate * window.getSize().x / duration.asSeconds();
        float y = window.getSize().y / 2 - samples[i] / 32768.0f * window.getSize().y / 2;
        waveform[i].position = sf::Vector2f(x, y);
        waveform[i].color = sf::Color::White;
    }

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(waveform);
        window.display();

        if (sound.getStatus() != sf::Sound::Playing) {
            window.close();
        }
    }
}

// Função para criar histograma dos valores de amplitude (T3)
void drawHistogram(sf::RenderWindow& window, const std::map<int, int>& histogram, const sf::Color& color, float offsetX, float offsetY, float width, float height) {
    int maxCount = 0;
    for (const auto& [bin, count] : histogram) {
        if (count > maxCount) {
            maxCount = count;
        }
    }

    float binWidth = width / histogram.size();
    int index = 0;

    for (const auto& [bin, count] : histogram) {
        float x = offsetX + index * binWidth;
        float y = offsetY + height - (count / static_cast<float>(maxCount)) * height;
        float barHeight = (count / static_cast<float>(maxCount)) * height;

        sf::RectangleShape bar(sf::Vector2f(binWidth - 2, barHeight));
        bar.setPosition(x, y);
        bar.setFillColor(color);
        
        window.draw(bar);
        index++;
    }
}


std::map<int, int> createHistogram(const sf::Int16* samples, std::size_t sampleCount, unsigned int channelCount, int binSize) {
    std::map<int, int> histogram;
    for (std::size_t i = 0; i < sampleCount; i += channelCount) {
        int sample = samples[i];
        int bin = (sample + 32768) / binSize;
        histogram[bin]++;
    }
    return histogram;
}

void createAmplitudeHistogram() {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(chosenFile)) {
        std::cerr << "Error loading audio file: " << chosenFile << std::endl;
        return;
    }

    unsigned int channelCount = buffer.getChannelCount();

    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    int binSize = 256;

    std::map<int, int> leftHistogram = createHistogram(samples, sampleCount, 1, binSize);
    std::map<int, int> rightHistogram, monoHistogram, sideHistogram;

    if (channelCount == 2) {
        rightHistogram = createHistogram(samples + 1, sampleCount, 2, binSize);
        for (std::size_t i = 0; i < sampleCount; i += 2) {
            int leftSample = samples[i];
            int rightSample = samples[i + 1];
            int monoSample = (leftSample + rightSample) / 2;
            int sideSample = (leftSample - rightSample) / 2;
            monoHistogram[(monoSample + 32768) / binSize]++;
            sideHistogram[(sideSample + 32768) / binSize]++;
        }
    }

    sf::RenderWindow window(sf::VideoMode(1200, 800), "Histogram of Audio Amplitude");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        drawHistogram(window, leftHistogram, sf::Color::Red, 50, 50, 500, 300);
        if (channelCount == 2) {
            drawHistogram(window, rightHistogram, sf::Color::Green, 650, 50, 500, 300);
            drawHistogram(window, monoHistogram, sf::Color::Blue, 50, 400, 500, 300);
            drawHistogram(window, sideHistogram, sf::Color::Yellow, 650, 400, 500, 300);
        }

        window.display();
    }
}

// Função para quantização uniforme do áudio (T4)
void quantizeAudio() {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(chosenFile)) {
        std::cerr << "Error loading audio file: " << chosenFile << std::endl;
        return;
    }

    unsigned int sampleRate = buffer.getSampleRate();
    unsigned int channelCount = buffer.getChannelCount();
    sf::Time duration = buffer.getDuration();

    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    std::vector<sf::Int16> quantizedSamples(sampleCount);

    int bits;
    std::cout << "Enter the number of bits for quantization (e.g., 8, 16): ";
    std::cin >> bits;

    int levels = std::pow(2, bits);
    int step = 65536 / levels;

    for (std::size_t i = 0; i < sampleCount; ++i) {
        quantizedSamples[i] = (samples[i] / step) * step;
    }

    sf::SoundBuffer quantizedBuffer;
    quantizedBuffer.loadFromSamples(quantizedSamples.data(), sampleCount, channelCount, sampleRate);

    sf::RenderWindow window(sf::VideoMode(1200, 600), "Waveform of Audio Sample");

    sf::VertexArray originalWaveform(sf::LinesStrip, sampleCount);
    sf::VertexArray quantizedWaveform(sf::LinesStrip, sampleCount);

    for (std::size_t i = 0; i < sampleCount; ++i) {
        float x = static_cast<float>(i) / sampleRate * window.getSize().x / duration.asSeconds();
        float yOriginal = window.getSize().y / 2 - samples[i] / 32768.0f * window.getSize().y / 2;
        float yQuantized = window.getSize().y / 2 - quantizedSamples[i] / 32768.0f * window.getSize().y / 2;
        originalWaveform[i].position = sf::Vector2f(x, yOriginal);
        originalWaveform[i].color = sf::Color::White;
        quantizedWaveform[i].position = sf::Vector2f(x, yQuantized);
        quantizedWaveform[i].color = sf::Color::Red;
    }

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    sf::Sound quantizedSound;
    quantizedSound.setBuffer(quantizedBuffer);
    quantizedSound.play();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(originalWaveform);
        window.draw(quantizedWaveform);
        window.display();

        if (sound.getStatus() != sf::Sound::Playing && quantizedSound.getStatus() != sf::Sound::Playing) {
            window.close();
        }
    }
}

// Função para calcular MSE e SNR entre amostras de áudio (T5)
void compareAudioSamples() {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(chosenFile)) {
        std::cerr << "Error loading audio file: " << chosenFile << std::endl;
        return;
    }

    const sf::Int16* samples = buffer.getSamples();
    std::size_t sampleCount = buffer.getSampleCount();

    std::vector<sf::Int16> quantizedSamples(sampleCount);

    int bits;
    std::cout << "Enter the number of bits for quantization (e.g., 8, 16): ";
    std::cin >> bits;

    int levels = std::pow(2, bits);
    int step = 65536 / levels;

    for (std::size_t i = 0; i < sampleCount; ++i) {
        quantizedSamples[i] = (samples[i] / step) * step;
    }

    double mse = 0.0;
    double signalPower = 0.0;
    double noisePower = 0.0;

    for (std::size_t i = 0; i < sampleCount; ++i) {
        double error = samples[i] - quantizedSamples[i];
        mse += error * error;
        signalPower += samples[i] * samples[i];
        noisePower += error * error;
    }

    mse /= sampleCount;
    double snr = 10 * std::log10(signalPower / noisePower);

    std::cout << "Mean Squared Error (MSE): " << mse << std::endl;
    std::cout << "Signal-to-Noise Ratio (SNR): " << snr << " dB" << std::endl;
}

// Função para exibir o menu e chamar as funções correspondentes
void displayMenu() {
    int choice;
    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. Load and display audio file information (T1)\n";
        std::cout << "2. Visualize audio waveform (T2)\n";
        std::cout << "3. Create amplitude histogram (T3)\n";
        std::cout << "4. Quantize audio and visualize waveform (T4)\n";
        std::cout << "5. Compare original and quantized audio samples (T5)\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                loadAudioFile();
                break;
            case 2:
                visualizeWaveform();
                break;
            case 3:
                createAmplitudeHistogram();
                break;
            case 4:
                quantizeAudio();
                break;
            case 5:
                compareAudioSamples();
                break;
            case 6:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cerr << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 6);
}

int main() {
    chooseAudioFile();
    displayMenu();
    return 0;
}