#include "enemy.h"
#include "affiliate/collider.h"
#include "raw/stats.h"

void Enemy::init()
{
    Actor::init();
    max_speed = 100.0f;
    _anim_normal = SpriteAnim::create(this, "assets/sprite/ghost-Sheet.png", 2.0f);
    _anim_hurt = SpriteAnim::create(this, "assets/sprite/ghostHurt-Sheet.png", 2.0f);
    _anim_die = SpriteAnim::create(this, "assets/sprite/ghostDead-Sheet.png", 2.0f);
    _current = _anim_normal;
    _current->setActive(true);
    _anim_hurt->setActive(false);
    _anim_die->setActive(false);
    _anim_die->setLoop(false);

    _collider = Collider::create(this, _anim_normal->getSize() / 1.0f);
    stats = Stats::create(this, 100.0f, 100.0f, 40.0f, 10.0f);

    setType(ObjectType::ENEMY);
}

void Enemy::aim_target(Player* target)
{
    if (target == nullptr || !isActive() || !_target->isActive()) {
        velocity = glm::vec2(0.0f);
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
    SDL_Log("die");
    if (!this->isAlive()) {
        changeStates(State::DIE);
    }
}

void Enemy::remove()
{
    if (_anim_die->getFinish()) {
        need_remove = true;
    }
}
void Enemy::attack()
{
    if (!_collider || !_target || _target->getCollider() == nullptr) {
        return;
    }
    if (_collider->isColliding(_target->getCollider())) {
        if (stats && _target->getStats()) {
            _target->takeDamage(stats->getDamage());
        }
    };
}

Enemy* Enemy::create(Object* parent, const glm::vec2& pos, Object* target)
{
    auto enemy = new Enemy();
    enemy->init();
    enemy->setPosition(pos);
    enemy->setTarget(static_cast<Player*>(target));
    if (parent) {
        parent->addChild(enemy);
    }
    return enemy;
}

void Enemy::update(float dt)
{
    if (!isActive()) {
        return;
    }
    Actor::update(dt);
    aim_target(_target);
    move(dt);
    attack();
    checkStates();
    remove();
}
