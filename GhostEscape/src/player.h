#ifndef PLAYER_H
#define PLAYER_H

#include "affiliate/sprite_anim.h"
#include "core/actor.h"
#include <SDL3/SDL_events.h>

class Player : public Actor {
protected:
    SpriteAnim* _sprite_idle;
    SpriteAnim* _sprite_move;
    bool is_moving = false;

public:
    virtual void init() override;
    virtual void update([[mayber_unused]] float dt) override;
    virtual void render() override;
    virtual void clean() override;
    virtual void handleEvents(SDL_Event& event) override;

    void keybordControl();
    void syncCamera();
    void checkStates();
    void changeStates(bool is_moving);
};

#endif