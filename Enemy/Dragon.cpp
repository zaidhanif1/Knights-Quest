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
    dragon_attack.setPosition(position);
    dragon_run.setPosition(position);
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
    dragon_idle.setPosition(position);
    dragon_attack.setPosition(position);
    dragon_run.setPosition(position);
    dragon_death.setPosition(position);
}

void Dragon::onLateUpdate(float delta_time)
{
    updateDragonAnimationState();
    updateAnimation(delta_time);
}

void Dragon::updateDragonAnimationState()
{
    DragonState currstate = dragon_state;
    setDragonAnimationState(currstate);
}

void Dragon::updateDragonState(const Player& player)
{
    float distanceToPlayer = std::abs(player.position.x - position.x);

    if (distanceToPlayer <= 200.f)
    {
        dragon_state = DragonState::DRAGON_ATTACK;
        velocity.x = 0.f;
    }
    else if (distanceToPlayer <= 300.f)
    {
        dragon_state = DragonState::DRAGON_WALK;

        if (player.position.x < position.x)
        {
            velocity.x = -100.f;
        }
        else
        {
            velocity.x = 100.f;
        }
    }
    else
    {
        dragon_state = DragonState::DRAGON_IDLE;
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

    switch (state)
    {
    case DragonState::DRAGON_IDLE:
        curr_animation = &dragon_idle;
        break;
    case DragonState::DRAGON_ATTACK:
        curr_animation = &dragon_attack;
        break;
    default:
        curr_animation = &dragon_idle;
        break;
    }
    return;
}