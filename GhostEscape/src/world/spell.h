#ifndef SPELL_H
#define SPELL_H

#include "../core/object_world.h"

class SpriteAnim;
class Spell : public ObjectWorld {
protected:
    SpriteAnim* _sprite = nullptr;
    float _damage = 60.0f;

public:
    static Spell* create(Object* parent, const std::string& path, const glm::vec2& pos, float damage = 60.0f, float scale = 1.0f, Anchor anchor = Anchor::CENTER);
    virtual void update(float dt) override;
    virtual void setSpriteAnim(SpriteAnim* sprite);

private:
    virtual void attack();
};

#endif