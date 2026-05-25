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
    Dragon* dragon = new Dragon({700, 150}, {1,1}, {256.f, 256.f});

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
    
    /** Create GameObject vector */
    std::vector<GameObject*> entities;
    entities.push_back(player);
    entities.push_back(dragon);

    /** Create platforms  */
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

        /** DRAGON FSM */
        dragon->updateDragonState(*player);
        
        /** Update all entities (physics + onUpdate) */
        for (auto* entity : entities)
        {
            entity->update(deltaTime);
        }
        
        /** Collision (any entity vs platforms) */
        for (auto* entity : entities)
        {
            for (auto& platform : platforms)
            {
                collisionHandler.handleCollision(*entity, platform);
            }
        }
        
        /** More collisions (any entity vs entity) */
        for (int i = 0; i < entities.size(); i++)
        {
            for (int j = i + 1; j < entities.size(); j++)
            {
                collisionHandler.handleCollision(*entities[i], *entities[j]);
            }   
        }
        


        /**  Late update (post-collision: animation state, etc.) */
        for (auto* entity : entities)
        {
            entity->lateUpdate(deltaTime);
        }

        /** Render everything */ 
        window.clear(sf::Color(135, 206, 235));
        
        for (auto& platform : platforms)
        {
            window.draw(platform.shape);
        }
        
        for (auto* entity : entities)
        {
            entity->draw(window);
        }
        
        window.display();
    }
    
    for (auto* entity : entities)
    {
        delete entity;
    }
    
    return 0;
}