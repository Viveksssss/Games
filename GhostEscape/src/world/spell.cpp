#include "spell.h"
#include "../affiliate/collider.h"
#include "../affiliate/sprite_anim.h"
#include "../core/scene.h"
#include "../enemy.h"

void Spell::update(float dt)
{
    ObjectWorld::update(dt);
    if (_sprite->getFinish()) {
        need_remove = true;
    }
    attack();
}

void Spell::setSpriteAnim(SpriteAnim* sprite)
{
    _sprite = sprite;
}

void Spell::attack()
{
    auto objects = game.getCurrentScene()->getChildrenObjectWorld();
    for (auto& obj : objects) {
        if (obj->getType() != ObjectType::ENEMY)
            continue;
        if (_collider && obj->getCollider() && _collider->isColliding(obj->getCollider())) {
            static_cast<Enemy*>(obj)->takeDamage(_damage);
        }
    }
}

Spell* Spell::create(Object*parent,const std::string&path,const glm::vec2 &pos,float damage,float scale,Anchor anchor)
{
    auto spell = new Spell();
    spell->init();
    spell->_damage = damage;
    spell->_sprite = SpriteAnim::create(spell,path,scale,anchor);
    spell->_sprite->setLoop(false);
    spell->_collider = Collider::create(spell,spell->_sprite->getSize());
    spell->setPosition(pos);
    if(parent)
        parent->addChild(spell);
    return spell;
}
