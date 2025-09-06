#include "weapon.h"
#include "../core/actor.h"
#include "../core/scene.h"
#include "../raw/stats.h"
#include "../world/spell.h"

void Weapon::update(float dt)
{
    Object::update(dt);
    _cool_down_timer += dt;
}

void Weapon::attack(const glm::vec2& pos, Spell* spell)
{
    if (!spell)
        return;
    parent->getStats()->useMana(_mana_cost);
    _cool_down_timer = 0.0f;
    spell->setPosition(pos);
    game.getCurrentScene()->safeAddChild(spell);
}

bool Weapon::canAttack()
{
    if (_cool_down_timer < _cool_down || !parent->getStats()->canUseMana(_mana_cost)) {
        return false;
    }
    return true;
}
