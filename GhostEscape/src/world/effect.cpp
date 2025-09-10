#include "effect.h"
#include "../core/scene.h"

void Effect::update(float dt)
{
    ObjectWorld::update(dt);
    checkFinished();
}

void Effect::checkFinished()
{
    if (_sprite->getFinish()) {
        need_remove = true;
        if (_next_world) {
            _next_world->setActive(true);
            game.getCurrentScene()->safeAddChild(_next_world);
            _next_world = nullptr;
        }
    }
}

Effect* Effect::create(Object* parent, const std::string& path, const glm::vec2& pos, float scale, ObjectWorld* next)
{
    auto effect = new Effect();
    effect->init();
    effect->_sprite = SpriteAnim::create(effect, path, scale);
    effect->_sprite->setLoop(false);
    effect->setPosition(pos);
    effect->setActive(true);
    effect->setNext(next);
    if (parent)
        parent->safeAddChild(effect);
    return effect;
}

void Effect::clean()
{
    ObjectWorld::clean();
    if (_next_world) { // 如果next_object_已经添加到场景中，就不应该再去删除
        _next_world->clean();
        delete _next_world;
        _next_world = nullptr;
    }
}