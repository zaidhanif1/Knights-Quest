#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../Animation/Animation.h"
#include "../GameObject/GameObject.h"

enum class PlayerState 
{
    IDLE,  ///< Player is standing still
    WALK,  ///< Player is walking
    RUN,   ///< Player is running
    JUMP,  ///< Player is jumping or in the air
};

namespace PlayerHitBox
{
    constexpr sf::Vector2f hitBoxDimensions{20, 40};
    constexpr sf::Vector2f hitBoxOffset{0.f, 5.f};
};

class Player : public GameObject
{
public:
    
    Player(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f frame_size);
    ~Player() = default;
    
    Animation idleAnimation;
    Animation walkAnimation;
    Animation runAnimation;
    Animation jumpAnimation;
    Animation* currentAnimation;

    PlayerState currentState;
    bool player_facing_right;
    
    const float WALK_SPEED = 200.0f;
    const float RUN_SPEED = 350.0f;
    const std::string base_path = "assets/Player/";
    
    bool load_all_animations();
    void jump();
    sf::FloatRect getGlobalBounds() const override;
    
    void setPosition(const sf::Vector2f& pos) override;
    void draw(sf::RenderWindow& window) override;
    const sf::Sprite& getSprite();
    
protected:
    void onUpdate(float delta_time) override;
    void onLateUpdate(float delta_time) override;

private:
    void setAnimation(PlayerState newState);
    void updateAnimationState();
    void updateAnimation(float deltaTime);
};
