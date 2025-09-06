#ifndef WEAPON_H
#define WEAPON_H

#include "../core/object.h"

class Spell;
class Actor;
class Weapon : public Object {
protected:
    Actor* parent;
    float _cool_down = 1.0f;
    float _mana_cost;
    float _cool_down_timer = 0.0f;

private:
public:
    virtual void update(float dt) override;
    inline void set_cool_down(float cool_down) { _cool_down = cool_down; }
    inline void set_mana_cost(float mana_cost) { _mana_cost = mana_cost; }
    inline float get_cool_down() { return _cool_down; }
    inline float get_mana_cost() { return _mana_cost; }
    inline void setParent(Actor* actor) { parent = actor; }
    inline Actor* getParent() { return parent; }
    virtual void attack(const glm::vec2& pos,Spell*spell);
    virtual bool canAttack();  
};

#endif // WEAPON_H