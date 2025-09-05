#ifndef EFFECT_H
#define EFFECT_H

#include "../affiliate/sprite_anim.h"
#include "../core/object_world.h"

#include <string>

class Effect : public ObjectWorld {
protected:
    // 特效的精灵动画
    SpriteAnim* _sprite = nullptr;
    // 动画结束后的下一个对象
    Object* _next_world = nullptr;

public:
    static Effect* create(Object* parent, const std::string& path, const glm::vec2& pos, float scale = 1.0f, ObjectWorld* next = nullptr);
    virtual void update(float dt) override;
    virtual void clean() override;

    inline void setNext(ObjectWorld* next) { _next_world = next; }
    inline Object* getNext() { return _next_world; }
    inline void setSprite(SpriteAnim* sprite) { _sprite = sprite; }
    inline SpriteAnim* getSprite() { return _sprite; }

private:
    // bool need_remov = false;
    void checkFinished();
};

#endif