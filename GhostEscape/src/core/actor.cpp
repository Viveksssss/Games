#include "actor.h"
#include "../affiliate/affiliate_bar.h"
#include "../raw/stats.h"
#include "scene.h"
void Actor::move(float dt)
{
    setPosition(getPosition() + velocity * dt);
    world_position = glm::clamp(world_position, glm::vec2(25), game.getCurrentScene()->getWorldSize() - glm::vec2(25));
}

void Actor::takeDamage(float damage)
{
    if (!stats)
        return;
    this->stats->takeDamage(damage);
}

bool Actor::isAlive()
{
    if (!this->stats)
        return false;
    return this->stats->getAlive();
}

void Actor::update(float dt)
{
    ObjectWorld::update(dt);
    updateHealth();
}

void Actor::updateHealth()
{
    if (!stats || !healthBar) {
        return;
    }
    healthBar->setPercentage(stats->getHealth() / stats->getMaxHealth());
}
