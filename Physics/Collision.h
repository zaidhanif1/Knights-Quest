#pragma once
#include <SFML/Graphics.hpp>
#include "../GameObject/GameObject.h"
#include "../Platform/Platform.h"
#include <optional>

/**
 * @class Collision
 * @brief Handles collision detection and resolution between GameObject and Platform objects
 * 
 * @note Collision resolution uses overlap size to determine collision direction:
 *       - Horizontal collisions occur when overlap width < overlap height
 *       - Vertical collisions occur when overlap width >= overlap height
 */
class Collision 
{
public:
    std::optional<sf::FloatRect> getIntersection( GameObject& entity,  Platform& platform);
    void handleCollision(GameObject& entity,  Platform& platform);

    //overloaded for two GameObject parameters. it is sloppy but works for now. 
    std::optional<sf::FloatRect> getIntersection( GameObject& a,  GameObject& b);
    void handleCollision(GameObject& a, GameObject& b);
};