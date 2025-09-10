#include "collider.h"
#include "../core/object_world.h"
bool Collider::isColliding(const Collider* other) const
{
    if (!other || !isActive() || !other->isActive())
        return false;
    if (type == ColliderType::CIRCLE && other->getColliderType() == ColliderType::CIRCLE) {
        auto point1 = _parent->getPosition() + _offset + getSize() / 2.0f;
        auto point2 = other->_parent->getPosition() + other->_offset + other->getSize() / 2.0f;
        return glm::length(point1 - point2) < ((getSize().x + other->getSize().x) / 2.0f);
    }
    // TODO: 矩形碰撞检测

    return false;
}

Collider* Collider::create(ObjectWorld* parent, glm::vec2 size, ColliderType type, Anchor anchor)
{
    auto collider = new Collider();
    collider->init();
    collider->setParent(parent);
    collider->setSize(size);
    collider->setColliderType(type);
    collider->setOffsetByAnchor(anchor);
    parent->addChild(collider);
    return collider;
}

void Collider::render()
{
#if OPEN_CIRCLE
    ObjectAffiliate::render();
    auto pos = _parent->getRenderPosition() + _offset;
    game.renderBox(pos, _size, 0.4);
#endif
}
