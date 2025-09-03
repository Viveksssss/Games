#ifndef ACTOR_H
#define ACTOR_H
#include "object_world.h"

class Actor : public ObjectWorld {
protected:
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
};

#endif