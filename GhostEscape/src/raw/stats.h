#ifndef STATS_H
#define STATS_H

#include "../core/actor.h"

class Actor;
class Stats : public Object {
protected:
    Actor* _parent;
    float _health = 100.0f;
    float _max_health = 100.0f;
    float _mana = 100.0f;
    float _max_mana = 100.0f;
    float _damage = 40.0f;
    float _mana_regen = 10.0f;

    bool is_alive = true;
    bool is_invincible = false;
    float invincible_time = 1.5f;
    float invincible_timer = 0.0f;

public:
    // 更新状态
    virtual void update(float dt) override;
    // 是否可以使用魔法
    bool canUseMana(float mana);
    // 恢复魔法
    void regenMana(float dt);
    // 受到伤害
    void takeDamage(float damage);
    // 使用魔法
    void useMana(float mana);
    // 创建
    static Stats* create(Actor* parent, float max_health = 100.0f, float max_mana = 100.0f, float damage = 40.0f, float mana_regen = 10.0f);

    inline float getHealth() { return _health; }
    inline float getMaxHealth() { return _max_health; }
    inline float getMana() { return _mana; }
    inline float getMaxMana() { return _max_mana; }
    inline float getDamage() { return _damage; }
    inline float getManaRegen() { return _mana_regen; }
    inline bool getAlive() { return is_alive; }
    inline void setHealth(float health) { _health = health; }
    inline void setMaxHealth(float max_health) { _max_health = max_health; }
    inline void setMana(float mana) { _mana = mana; }
    inline void setMaxMana(float max_mana) { _max_mana = max_mana; }
    inline void setDamage(float damage) { _damage = damage; }
    inline void setManaRegen(float mana_regen) { _mana_regen = mana_regen; }
    inline void setAlive(bool alive) { is_alive = alive; }
    inline Actor* getParent() { return _parent; }
    inline void setParent(Actor* parent) { this->_parent = parent; }
    inline bool getInvincible() { return is_invincible; }
    inline void setInvincible(bool invincible) { is_invincible = invincible; }
    inline float getInvincibleTime() { return invincible_time; }
    inline void setInvincibleTime(float time) { invincible_time = time; }
    void updateInvincibleTimer(float dt);
};

#endif