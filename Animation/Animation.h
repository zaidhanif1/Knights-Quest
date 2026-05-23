#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>


class Animation
{
public:
    Animation();

    bool loadFromFile(const std::string& filename, 
                      const sf::Vector2f& frame_size, 
                      unsigned int frame_count,
                      float fps,
                      sf::Vector2f position);

    void update(float deltaTime);
    void setFrame(unsigned int frame);
    void play();
    void pause();
    void stop();
    void setfps(float speed);
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;
    void setScale(const sf::Vector2f& scale);
    void setOrigin(const sf::Vector2f& origin);
    const sf::Sprite& getSprite() const;
    std::optional<sf::Sprite> sprite;
    sf::Texture texture;    
    sf::Vector2f frameSize;
    unsigned int frameCount;
    unsigned int currentFrame;
    float fps;
    float frameTime;
    bool isPlaying;
    bool loop;
    bool textureLoaded;
    void updateTextureRect();
};
