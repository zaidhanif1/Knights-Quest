#include "Player.h"
#include <iostream>
#include <cmath>

Player::Player(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f frame_size) 
    :
GameObject(position, velocity, frame_size)
    {
        this->player_facing_right = true;
        this->currentState = PlayerState::IDLE;
    }   
bool Player::load_all_animations()
{
    bool success = true;
    
    // Load all animations
    success &= idleAnimation.loadFromFile(base_path + "IDLE.png", this->framesize, 6, 8.0f, this->position);
    success &= walkAnimation.loadFromFile(base_path + "WALK.png", this->framesize, 6, 8.0f, this->position);
    success &= runAnimation.loadFromFile(base_path + "RUN.png", this->framesize, 6, 8.0f, this->position);
    success &= jumpAnimation.loadFromFile(base_path + "JUMP.png", this->framesize, 5, 8.0f, this->position);

    if (!success) return false;

    // Set initial animation
    currentAnimation = &idleAnimation;
    
    // Start the idle animation
    idleAnimation.play();
    
    return success;
}


void Player::onUpdate(float delta_time) 
    {
        if (player_facing_right != facing_right) 
        {
            sf::Vector2f scale = facing_right ? sf::Vector2f(1.0f, 1.0f) : sf::Vector2f(-1.0f, 1.0f);
            idleAnimation.setScale(scale);
            walkAnimation.setScale(scale);
            runAnimation.setScale(scale);
            jumpAnimation.setScale(scale);
        }
        
        player_facing_right = facing_right;
        
        idleAnimation.setPosition(position);
        walkAnimation.setPosition(position);
        runAnimation.setPosition(position);
        jumpAnimation.setPosition(position);

    }


void Player::updateAnimationState()
{
    // Determine which animation should be playing based on player state
    PlayerState newState = currentState;
    
    // Priority order: JUMP > RUN > WALK > IDLE
    if (!onGround) 
    {
        newState = PlayerState::JUMP;
    } 
    else if (std::abs(velocity.x) > 250.0f) 
    {  // Run threshold (moveSpeed is 300, so this catches running)
        newState = PlayerState::RUN;
    } 
    else if (std::abs(velocity.x) > 10.0f) 
    {  // Walk threshold 
        newState = PlayerState::WALK;
    } 
    else 
    {
        newState = PlayerState::IDLE;
    }
    
    // Only switch if state actually changed
    if (newState != currentState) 
    {
        setAnimation(newState);
    }
}

void Player::updateAnimation(float delta_time)
{
    // Update the current animation - call this AFTER updateAnimationState()
    if (currentAnimation) 
    {
        currentAnimation->update(delta_time);
    }
}

void Player::handleInput()
{
    velocity.x = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) 
    {
        velocity.x = -RUN_SPEED; 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) 
    {
        velocity.x = RUN_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) 
    {
        jump();
    }
}

void Player::setAnimation(PlayerState newState)
{
    // don't do anything if already in this state
    
    currentState = newState;
    
    // switch to new animation
    switch (newState) 
    {
        case PlayerState::IDLE:
            currentAnimation = &idleAnimation;
            break;
        case PlayerState::WALK:
            currentAnimation = &walkAnimation;
            break;
        case PlayerState::RUN:
            currentAnimation = &runAnimation;
            break;
        case PlayerState::JUMP:
            currentAnimation = &jumpAnimation;
            break;
    }
    
    // reset the animation to start fresh - this prevents showing invalid/blank frames
    // when switching between animations with different frame counts
    if (currentAnimation) 
    {
        currentAnimation->currentFrame = 0;
        currentAnimation->frameTime = 0.0f;
        currentAnimation->updateTextureRect();
        currentAnimation->play();
    }
}

void Player::jump() 
{
    if (onGround) 
    {
        velocity.y = -470.0f;
        onGround = false;
    }
}

sf::FloatRect Player::getGlobalBounds() const 
{
    // Calculate custom collision box centered on the player's position
    // The box is offset from center: top-left corner is at (center.x - width/2, center.y - height/2)
    // We offset the Y slightly toward the bottom since feet should be the reference point
    return sf::FloatRect
    (
        sf::Vector2f(
            position.x - (PlayerHitBox::hitBoxDimensions.x / 2.0f) + PlayerHitBox::hitBoxOffset.x, 
            position.y - (PlayerHitBox::hitBoxDimensions.y / 2.0f) + PlayerHitBox::hitBoxOffset.y),
        PlayerHitBox::hitBoxDimensions
    );
}


void Player::setPosition(const sf::Vector2f& pos) 
{
    position = pos;
    // Sync all animation positions
    idleAnimation.setPosition(position);
    walkAnimation.setPosition(position);
    runAnimation.setPosition(position);
    jumpAnimation.setPosition(position);
}

void Player::onLateUpdate(float delta_time)
{
    updateAnimationState();
    updateAnimation(delta_time);
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(getSprite());
    return;
}

const sf::Sprite& Player::getSprite()
{
    if (currentAnimation) 
    {
        return currentAnimation->getSprite();
    }
    return idleAnimation.getSprite();
}