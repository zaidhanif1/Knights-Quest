#include "Dragon.h"
#include "Enemy.h"

Dragon::Dragon(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f frame_size)
    : Enemy(position, velocity, frame_size)
{
    this->dragon_facing_right = true;
    this->dragon_state = DragonState::DRAGON_IDLE;
    this->curr_animation = nullptr;
    
}

bool Dragon::load_dragon_animations()
{
    bool success = true;

    success &= this->dragon_idle.loadFromFile(base_path + "Idle.png", this->framesize, DRAGON_IDLE_FRAME_COUNT, 6.0, this->position);
    success &= this->dragon_death.loadFromFile(base_path + "Death.png", this->framesize, DRAGON_DEATH_FRAME_COUNT, 6.0, this->position);
    success &= this->dragon_walk.loadFromFile(base_path + "Walk.png", this->framesize, DRAGON_WALK_FRAME_COUNT, 6.0, this->position);
    success &= this->dragon_attack.loadFromFile(base_path + "Attack.png", this->framesize, DRAGON_ATTACK_FRAME_COUNT, 4.0, this->position);

    if (!success) return false;

    curr_animation = &dragon_idle;
    dragon_idle.play();

    return success;
}

void Dragon::draw(sf::RenderWindow &window)
{
    if (curr_animation)
    {
        window.draw(curr_animation->getSprite());
        return;
    }
    window.draw(dragon_idle.getSprite());
}

void Dragon::setPosition(const sf::Vector2f &pos)
{
    position = pos;
    dragon_idle.setPosition(position);
    dragon_walk.setPosition(position);
    dragon_attack.setPosition(position);
    dragon_death.setPosition(position);
}

sf::FloatRect Dragon::getGlobalBounds() const
{
    return sf::FloatRect(
        sf::Vector2f(
            (position.x + DragonHitBox::hitBoxOffset.x )- (DragonHitBox::hitBoxDimensions.x / 2.f),
            (position.y + DragonHitBox::hitBoxOffset.y) - (DragonHitBox::hitBoxDimensions.y / 2.f)),
        DragonHitBox::hitBoxDimensions
    );
}


void Dragon::onUpdate(float delta_time)
{
    if (dragon_facing_right != facing_right)
    {
        sf::Vector2f scale = facing_right ? sf::Vector2f(1.f, 1.f) : sf::Vector2f(-1.f, 1.f);
        dragon_idle.setScale(scale);
        dragon_walk.setScale(scale);
        dragon_death.setScale(scale);
        dragon_attack.setScale(scale);
    }
    dragon_facing_right = facing_right;

    dragon_idle.setPosition(position);
    dragon_attack.setPosition(position);
    dragon_walk.setPosition(position);
    dragon_death.setPosition(position);
}

void Dragon::onLateUpdate(float delta_time)
{
    setDragonAnimationState(dragon_state);
    updateAnimation(delta_time);
}


void Dragon::updateDragonState(const Player& player)
{
    float distanceToPlayer = std::abs(player.position.x - position.x);

    if (distanceToPlayer <= 200.f)
    {
        dragon_state = DragonState::DRAGON_ATTACK;
        setDragonAnimationState(dragon_state);
        velocity.x = 0.f;
    }
    else if (distanceToPlayer <= 300.f)
    {
        dragon_state = DragonState::DRAGON_WALK;
        setDragonAnimationState(dragon_state);
        if (player.position.x < position.x)
        {
            velocity.x = -100.f; //player is to the keft
        }
        else
        {
            velocity.x = 100.f; //player is to the right
        }
    }
    else
    {
        dragon_state = DragonState::DRAGON_IDLE;
        setDragonAnimationState(dragon_state);
        velocity.x = 0.f;
    }
}

void Dragon::updateAnimation(float delta_time)
{
    if (curr_animation)
    {
        curr_animation->update(delta_time);
    }
}
void Dragon::setDragonAnimationState(DragonState state)
{
    Animation* next_animation = curr_animation;

    switch (state)
    {
    case DragonState::DRAGON_IDLE:
        next_animation = &dragon_idle;
        break;
    case DragonState::DRAGON_WALK:
        next_animation = &dragon_walk;
        break;
    case DragonState::DRAGON_ATTACK:
        next_animation = &dragon_attack;
        break;
    case DragonState::DRAGON_DEATH:
        next_animation = &dragon_death;
        break;
    default:
        next_animation = &dragon_idle;
        break;
    }

    if (next_animation && next_animation != curr_animation)
    {
        curr_animation = next_animation;
        curr_animation->currentFrame = 0;
        curr_animation->frameTime = 0.0f;
        curr_animation->updateTextureRect();
        curr_animation->play();
    }

    return;
}
