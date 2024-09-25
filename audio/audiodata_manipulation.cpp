#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <iomanip> 
#include <vector>

int main() {
    sf::SoundBuffer buffer;
    int sampleNumber;

    std::cout << "Choose a sample number between 1 and 7: ";
    std::cin >> sampleNumber;

    if (sampleNumber < 1 || sampleNumber > 7) {
        std::cerr << "Invalid sample number." << std::endl;
        return -1;
    }

    std::string fileName = "audio_files/sample0" + std::to_string(sampleNumber) + ".wav";

    if (!buffer.loadFromFile(fileName)) {
        std::cerr << "Error loading audio file: " << fileName << std::endl;
        return -1;
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

    // new - start

    sf::RenderWindow window(sf::VideoMode(1200, 600), "Waveform of Audio Sample");

    sf::VertexArray waveform(sf::LinesStrip, sampleCount);

    for (std::size_t i = 0; i < sampleCount; ++i) {
        // x-axis represents time
        float x = static_cast<float>(i) / sampleRate * window.getSize().x / duration.asSeconds();
        // y-axis represents amplitude
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

    // new - end

    return 0;
}


/* dizer ao professor que fiz a instalação do sfml em vez de usar a pasta que ele disponibilizou uma vez que 
era para windows e eu quero trabalhar em linux :) */
