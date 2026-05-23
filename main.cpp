#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include "Player/Player.h"
#include "Platform/Platform.h"
#include "Physics/Collision.h"
#include "GameObject/GameObject.h"
#include "Enemy/Dragon.h"
#include <iostream>


int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "SFML Game");
    window.setFramerateLimit(60);

    Player* player = new Player({1, 1}, {1,1}, {96,70});
    Dragon* dragon = new Dragon({150, 150}, {1,1}, {256.f, 256.f});

    if (!player->load_all_animations()) 
    {
        std::cerr << "Failed to load player animations!" << std::endl;
        delete player;
        return -1;
    }
    if (!dragon->load_dragon_animations()) 
    {
        std::cerr << "Failed to load dragon animations!" << std::endl;
        delete dragon;
        return -1;
    }
    
    
    std::vector<GameObject*> entities;
    entities.push_back(player);
    entities.push_back(dragon);

    std::vector<Platform> platforms;
    Platform::createPlatforms(platforms);
    
    Collision collisionHandler;
    sf::Clock clock;
    
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) 
            {
                window.close();
            }
        }
        
        // Input (Player-specific)
        player->velocity.x = 0;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) 
        {
            player->velocity.x = -player->RUN_SPEED; 
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) 
        {
            player->velocity.x = player->RUN_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) 
        {
            player->jump();
        }
        
        // Update all entities (physics + onUpdate)
        for (auto* entity : entities)
        {
            entity->update(deltaTime);
        }
        
        // Collision (any entity vs platforms)
        for (auto* entity : entities)
        {
            for (auto& platform : platforms)
            {
                collisionHandler.handleCollision(*entity, platform);
            }
        }

        for (int i = 0; i < entities.size(); i++)
        {
            for (int j = i + 1; j < entities.size(); j++)
            {
                collisionHandler.handleCollision(*entities[i], *entities[j]);
            }   
        }
        
        

        // Boundary clamping (Player-specific for now)
        //TODO: MOVE INTO UPDATE LOGIC
        sf::Vector2f playerPos = player->getPosition();
        if (playerPos.x < 0) 
        {
            player->setPosition(sf::Vector2f(0, playerPos.y));
        }
        if (playerPos.x > 800) 
        {
            player->setPosition(sf::Vector2f(800, playerPos.y));
        }


        // Late update (post-collision: animation state, etc.)
        for (auto* entity : entities)
        {
            entity->lateUpdate(deltaTime);
        }
        sf::FloatRect bounds = dragon->getGlobalBounds();

        sf::RectangleShape hitbox;


        hitbox.setPosition(bounds.position);
        hitbox.setSize(bounds.size);
        hitbox.setFillColor(sf::Color::Transparent);
        hitbox.setOutlineColor(sf::Color::Red);
        hitbox.setOutlineThickness(1.f);

        // Render
        window.clear(sf::Color(135, 206, 235));
        
        for (auto& platform : platforms)
        {
            window.draw(platform.shape);
        }
        
        for (auto* entity : entities)
        {
            entity->draw(window);
        }
        window.draw(hitbox);
        
        window.display();
    }
    
    for (auto* entity : entities)
    {
        delete entity;
    }
    
    return 0;
}