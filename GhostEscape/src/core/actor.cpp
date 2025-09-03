#include "actor.h"
#include "scene.h"

void Actor::move(float dt)
{
    setPosition(getPosition() + velocity * dt);
    world_position = glm::clamp(world_position, glm::vec2(0), game.getCurrentScene()->getWorldSize());
}