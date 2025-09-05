#ifndef ACTOR_H
#define ACTOR_H
#include "object_world.h"

class Stats;
class Actor : public ObjectWorld {
protected:
    // 属性
    Stats* stats = nullptr;
    // 当前的速度矢量
    glm::vec2 velocity = glm::vec2(0, 0);
    // 最大速度
    float max_speed = 100.0f;

public:
    void move(float dt);

    inline glm::vec2 getVelocity() const { return velocity; };
    inline void setVelocity(const glm::vec2& value) { velocity = value; }
    inline float getMaxSpeed() const { return max_speed; }
    inline void setMaxSpeed(float value) { max_speed = value; }
    inline Stats* getStats() const { return stats; }
    inline void setStats(Stats* value) { stats = value; }
    virtual void takeDamage(float damage);
    virtual bool isAlive();
};

#endif