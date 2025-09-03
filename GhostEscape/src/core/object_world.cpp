#include "object_world.h"
#include "scene.h"
void ObjectWorld::update(float dt)
{
    ObjectScreen::update(dt);
    render_position = game.getCurrentScene()->worldToScreen(world_position);
}
glm::vec2 ObjectWorld::getPosition() const
{
    return world_position;
}

void ObjectWorld::setPosition(const glm::vec2& pos)
{
    world_position = pos;
    render_position = game.getCurrentScene()->worldToScreen(pos);
}

void ObjectWorld::setRenderPosition(const glm::vec2& pos)
{
    render_position = pos;
    world_position = game.getCurrentScene()->screenToWorld(pos);
}
