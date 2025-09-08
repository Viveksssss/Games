#ifndef PLAYER_H
#define PLAYER_H

#include "affiliate/sprite_anim.h"
#include "core/actor.h"
#include "raw/weapon.h"
#include <SDL3/SDL_events.h>

class WeaponThunder;
class Effect;
class Player : public Actor {
protected:
    SpriteAnim* _sprite_idle;
    SpriteAnim* _sprite_move;
    Effect* _effect_die;
    bool is_moving = false;

    WeaponThunder* _weapon_thunder = nullptr;

public:
    virtual void init() override;
    virtual void update([[maybe_unused]] float dt) override;
    virtual void render() override;
    virtual void clean() override;
    virtual bool handleEvents(SDL_Event& event) override;
    virtual void takeDamage(float damage) override;

    void keybordControl();
    void syncCamera();
    void checkStates();
    void changeStates(bool is_moving);
    void checkDead();
};

#endif