#ifndef HUD_STATS_H
#define HUD_STATS_H

#include "../core/object_screen.h"

class Sprite;
class Actor;
class HUDStats : public ObjectScreen {
protected:
    Sprite* _health_bar = nullptr;
    Sprite* _health_bg = nullptr;
    Sprite* _health_icon = nullptr;

    Sprite* _mana_bar = nullptr;
    Sprite* _mana_bg = nullptr;
    Sprite* _mana_icon = nullptr;

    float _health = 0.0f;
    float _mana = 0.0f;

    Actor* _target = nullptr;

public:
    static HUDStats* create(Object* parent, Actor* target, const glm::vec2& render_position);

    virtual void render() override;
    virtual void update(float dt) override;
    virtual void init() override;

    void updateHealth();
    void updateMana();

    inline void setHealth(float health) { _health = health; }
    inline void setMana(float mana) { _mana = mana; }
    inline float getHealth() { return _health; }
    inline float getMana() { return _mana; }
    inline void setHealthBar(Sprite* health_bar) { _health_bar = health_bar; }
    inline void setHealthBg(Sprite* health_bg) { _health_bg = health_bg; }
    inline void setHealthIcon(Sprite* health_icon) { _health_icon = health_icon; }
    inline Sprite* getHealthBar() { return _health_bar; }
    inline Sprite* getHealthBg() { return _health_bg; }
    inline Sprite* getHealthIcon() { return _health_icon; }
    inline void setTarget(Actor* target) { _target = target; }
    inline Actor* getTarget() { return _target; }
};

#endif