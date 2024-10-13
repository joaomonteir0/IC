#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <map>
#include <SFML/Graphics.hpp>

void saveScreenshot(sf::RenderWindow& window, const std::string& filename);
void displayCharacterHistogram(const std::map<wchar_t, int>& histogram);
void displayWordHistogram(const std::map<std::wstring, int>& freqMap);

#endif