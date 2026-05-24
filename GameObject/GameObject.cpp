#include "GameObject.h"

GameObject::GameObject(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f framesize)
{   
    this->position = position;
    this->velocity = velocity;
    this->framesize = framesize;
    this->hit_box_dimensions= hit_box_dimensions;
    onGround = false;
    facing_right = true;
}

void GameObject::update(float delta_time)
{
    handleInput();
    if (!onGround) 
    {
        velocity.y += GRAVITY * delta_time;
    }
    position.x += velocity.x * delta_time;
    position.y += velocity.y * delta_time;

    if (velocity.x > 0.1f) 
    {
        facing_right = true;
    } 
    else if (velocity.x < -0.1f) 
    {
        facing_right = false;
    }
    if (position.x < 0) 
    {
        setPosition(sf::Vector2f(0, position.y));
    }
    if (position.x > 800) 
    {
        setPosition(sf::Vector2f(800, position.y));
    }
    
    onGround = false;

    onUpdate(delta_time);
}

void GameObject::onUpdate(float delta_time)
{
}

void GameObject::lateUpdate(float delta_time)
{
    onLateUpdate(delta_time);
}

void GameObject::onLateUpdate(float delta_time)
{
}

void GameObject::handleInput()
{
}

void GameObject::draw(sf::RenderWindow& window)
{
}

sf::Vector2f GameObject::getPosition() const
{
    return position;
}

void GameObject::setPosition(const sf::Vector2f& pos)
{
    position = pos;
}

sf::FloatRect GameObject::getGlobalBounds() const 
{
    return sf::FloatRect(
        sf::Vector2f(position.x - (hit_box_dimensions.x / 2.0f), position.y - (hit_box_dimensions.y / 2.0f)),
        hit_box_dimensions
    );
}
