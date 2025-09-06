#include "stats.h"
void Stats::update(float dt)
{
    Object::update(dt);
    regenMana(dt);
    updateInvincibleTimer(dt);
}

bool Stats::canUseMana(float mana)
{
    return _mana >= mana;
}

void Stats::regenMana(float dt)
{
    _mana += _mana_regen * dt;
    if (_mana > _max_mana) {
        _mana = _max_mana;
    }
}

void Stats::takeDamage(float damage)
{
    if (is_invincible)
        return;
    _health -= damage;
    if (_health <= 0) {
        _health = 0;
        is_alive = false;
    }
    is_invincible = true;
}

void Stats::useMana(float mana)
{
    _mana -= mana;
    if (mana < 0) {
        _mana = 0;
    }
}

Stats* Stats::create(Actor* parent, float max_health, float max_mana, float damage, float mana_regen)
{
    Stats* stats = new Stats();
    stats->_parent = parent;
    stats->_max_health = max_health;
    stats->_max_mana = max_mana;
    stats->_damage = damage;
    stats->_mana_regen = mana_regen;
    stats->_health = max_health;
    stats->_mana = max_mana;
    parent->addChild(stats);
    return stats;
}

void Stats::updateInvincibleTimer(float dt)
{
    if (is_invincible) {
        invincible_timer += dt;
        if (invincible_timer >= invincible_time) {
            is_invincible = false;
            invincible_timer = 0;
        }
    }
}
