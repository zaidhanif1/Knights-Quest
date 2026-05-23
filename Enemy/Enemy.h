#pragma once
#include <SFML/Graphics.hpp>
#include "../GameObject/GameObject.h"


enum class EnemyType
{
    DRAGON,
    DEMON,
    MEDUSA
};



class Enemy : public GameObject
{
    public:
        Enemy(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f frame_size);
};