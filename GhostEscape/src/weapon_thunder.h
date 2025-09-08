#ifndef WEAPON_THUNDER_H
#define WEAPON_THUNDER_H

#include "raw/weapon.h"

class HUDSkill;
class WeaponThunder : public Weapon {
public:
    virtual bool handleEvents(SDL_Event& event) override;
    virtual void init() override;
    virtual void update(float delta) override;

    static WeaponThunder* create(Actor* parent, float cool_down, float mana_cost);

protected:
    HUDSkill* _hud_skill = nullptr;
};

#endif