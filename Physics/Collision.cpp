#include <optional>
#include "Collision.h"


std::optional<sf::FloatRect> Collision::getIntersection( GameObject& entity,  Platform& platform) 
{
    sf::FloatRect entityBounds = entity.getGlobalBounds();
    sf::FloatRect platformBounds = platform.shape.getGlobalBounds();
    
    return entityBounds.findIntersection(platformBounds);
}


std::optional<sf::FloatRect> Collision::getIntersection( GameObject& a,  GameObject& b)
{
    sf::FloatRect a_bounds = a.getGlobalBounds();
    sf::FloatRect b_bounds = b.getGlobalBounds();
    
    return a_bounds.findIntersection(b_bounds);

}

void Collision::handleCollision(GameObject& entity,  Platform& platform) 
{
    std::optional<sf::FloatRect> intersection = getIntersection(entity, platform);
    
    if (!intersection.has_value()) 
    {
        return;
    }
    
    sf::FloatRect overlap = intersection.value();

    sf::Vector2f entityPos = entity.getPosition();
    sf::FloatRect entityBounds = entity.getGlobalBounds();
    sf::FloatRect platformBounds = platform.shape.getGlobalBounds();
    
    if (overlap.size.x < overlap.size.y) 
    {
        // Horizontal collision (left/right)
        if (entityBounds.position.x < platformBounds.position.x) 
        {
            entity.setPosition(
                sf::Vector2f(entityPos.x - overlap.size.x, entityPos.y)
            );
        } 
        else 
        {
            entity.setPosition(
                sf::Vector2f(entityPos.x + overlap.size.x, entityPos.y)
            );
        }
        entity.velocity.x = 0;
    } 
    else 
    {
        // Vertical collision (top/bottom)
        if (entityBounds.position.y < platformBounds.position.y) 
        {
            // Landed on top — tiny overlap prevents onGround flicker
            entity.setPosition(
                sf::Vector2f(entityPos.x, entityPos.y - overlap.size.y + 0.1f)
            );
            entity.velocity.y = 0;
            entity.onGround = true;
        } 
        else 
        {
            // Hit from below
            entity.setPosition(
                sf::Vector2f(entityPos.x, entityPos.y + overlap.size.y)
            );
            entity.velocity.y = 0;
        }
    }
}
void Collision::handleCollision(GameObject& a, GameObject& b) 
{
    std::optional<sf::FloatRect> intersection = getIntersection(a, b);
    
    if (!intersection.has_value()) 
    {
        return;
    }
    
    sf::FloatRect overlap = intersection.value();

    sf::Vector2f a_pos = a.getPosition();
    sf::FloatRect a_bounds = a.getGlobalBounds();
    sf::Vector2f b_pos = b.getPosition();
    sf::FloatRect b_bounds = b.getGlobalBounds();
    
    if (overlap.size.x < overlap.size.y) 
    {
        // Horizontal collision (left/right)
        if (a_pos.x < b_pos.x) 
        {
            a.setPosition(
                sf::Vector2f(a_pos.x - overlap.size.x, a_pos.y)
            );
            b.setPosition(
                sf::Vector2f(b_pos.x + overlap.size.x, b_pos.y)
            );
        } 
        else 
        {
            a.setPosition(
                sf::Vector2f(a_pos.x + overlap.size.x, a_pos.y)
            );
            b.setPosition(
                sf::Vector2f(b_pos.x - overlap.size.x, b_pos.y)
            );
        }
        a.velocity.x = 0;
        b.velocity.x = 0;
    } 
    else 
    {
        // Vertical collision (top/bottom)
        if (a_pos.y < b_pos.y) 
        {
            // a landed on top
            a.setPosition(
                sf::Vector2f(a_pos.x, a_pos.y - overlap.size.y) //above
            );
            a.velocity.y = 0;
        } 
        else 
        {
            //and collided with b from the bottom 
            a.setPosition(
                sf::Vector2f(a_pos.x, a_pos.y + overlap.size.y) //offset b DOWN
            );
            b.setPosition(
                sf::Vector2f(b_pos.x, b_pos.y - overlap.size.y) //offset b UP
            );
            a.velocity.y = 0;
            b.velocity.y = 0;
        }
    }
}