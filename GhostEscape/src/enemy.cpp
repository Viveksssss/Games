#include "enemy.h"

void Enemy::aim_target(Player* target)
{
    if (target == nullptr) {
        return;
    }
    _target = target;
    auto direction = _target->getPosition() - getPosition();
    direction = glm::normalize(direction);
    velocity = direction * max_speed;
}

void Enemy::setTarget(Player* target)
{
    _target = target;
}

Player* Enemy::getTarget() const
{
    return _target;
}

void Enemy::changeStates(State new_state)
{
    _current->setActive(false);
    switch (new_state) {
    case State::NORMAL:
        _current = _anim_normal;
        _current->setActive(true);
        break;
    case State::HURT:
        _current = _anim_hurt;
        _current->setActive(true);
        break;
    case State::DIE:
        _current = _anim_die;
        _current->setActive(true);
        break;
    }
    _state = new_state;
}

Enemy::State Enemy::getStates() const
{
    return _state;
}

void Enemy::checkStates()
{
}

void Enemy::remove()
{
    if (_anim_die->getFinish()) {
        need_remove = true;
    }
}

void Enemy::init()
{
    Actor::init();
    _anim_normal = SpriteAnim::create(this, "assets/sprite/ghost-Sheet.png", 2.0f);
    _anim_hurt = SpriteAnim::create(this, "assets/sprite/ghostHurt-Sheet.png", 2.0f);
    _anim_die = SpriteAnim::create(this, "assets/sprite/ghostDead-Sheet.png", 2.0f);
    _current = _anim_normal;
    _current->setActive(true);
    _anim_hurt->setActive(false);
    _anim_die->setActive(false);
    _anim_die->setLoop(false);
}

void Enemy::update(float dt)
{
    Actor::update(dt);
    aim_target(_target);
    move(dt);
}
