#ifndef UI_MOUSE_H
#define UI_MOUSE_H

#include "../affiliate/sprite.h"
#include "../core/object_screen.h"

class UIMouse : public ObjectScreen {
protected:
    Sprite* _sprite1 = nullptr;
    Sprite* _sprite2 = nullptr;
    float timer = 0;
    const float switch_time = 0.2f;

public:
    static UIMouse* create(Object* parent, const std::string& path1, const std::string& path2, float scale = 1.0f, Anchor anchor = Anchor::CENTER);

    Sprite* getSprite1() { return _sprite1; }
    Sprite* getSprite2() { return _sprite2; }
    void setSprite1(Sprite* sprite) { _sprite1 = sprite; }
    void setSprite2(Sprite* sprite) { _sprite2 = sprite; }

    virtual void update(float dt) override;
};

#endif // UI_MOUSE_H