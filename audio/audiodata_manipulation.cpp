#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <iomanip> 

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

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    while (sound.getStatus() == sf::Sound::Playing) {
        sf::sleep(sf::seconds(0.1f));
    }

    return 0;
}


/* dizer ao professor que fiz a instalação do sfml em vez de usar a pasta que ele disponibilizou uma vez que 
era para windows e eu quero trabalhar em linux :) */
