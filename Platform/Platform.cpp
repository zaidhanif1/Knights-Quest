#include "Platform.h"
#include <SFML/Graphics.hpp>
#include <iostream>

    
    Platform::Platform(float x, float y, float width, float height, sf::Color color) 
    {
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(sf::Vector2f(x, y));
        shape.setFillColor(color);
    }

    void Platform::createPlatforms(std::vector<Platform> &platforms)
    {
        platforms.push_back(Platform(0, 550, 1000, 50, sf::Color(0, 100, 0)));      // Ground
        platforms.push_back(Platform(200, 450, 150, 20, sf::Color::Black));    // Platform 1
        platforms.push_back(Platform(400, 350, 150, 20, sf::Color::Black));    // Platform 2
        platforms.push_back(Platform(600, 250, 150, 20, sf::Color::Black));    // Platform 3

    }
