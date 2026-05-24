#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>


class GameObject
{
    public:
        GameObject(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f framesize);
        virtual ~GameObject() = default;

        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f framesize;
        sf::Vector2f hit_box_dimensions;
        bool onGround;
        bool facing_right;

        void update(float delta_time);
        void lateUpdate(float delta_time);
        virtual void draw(sf::RenderWindow& window);

        sf::Vector2f getPosition() const;
        virtual void setPosition(const sf::Vector2f& pos);
        virtual sf::FloatRect getGlobalBounds() const;

        const float GRAVITY = 980.f;

    protected:
        //player class overrides
        virtual void onUpdate(float delta_time); //sync object visuals after movement
        virtual void onLateUpdate(float delta_time); //make final decisions after collisions and other objects may have affected state
        virtual void handleInput();
};

#endif // GAMEOBJECT_H

