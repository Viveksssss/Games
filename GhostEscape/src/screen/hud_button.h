#ifndef HUD_BUTTON_H
#define HUD_BUTTON_H

#include "../affiliate/sprite.h"
#include "../core/object_screen.h"

class HUDButton : public ObjectScreen {
protected:
    Sprite* _sprite_normal = nullptr;
    Sprite* _sprite_hover = nullptr;
    Sprite* _sprite_press = nullptr;

    bool is_hover = false;
    bool is_press = false;
    bool is_trigger = false;

public:
    static HUDButton* create(Object* parent, const glm::vec2& pos, const std::string& file1, const std::string& file2, const std::string& file3, float scale = 1.0f, Anchor anchor = Anchor::CENTER);
    virtual bool handleEvents(SDL_Event& event) override;
    virtual void update(float dt) override;
    // setter and getter
    // inline void setSpriteNormal(Sprite* sprite) { _sprite_normal = sprite; }
    // inline void setSpriteHover(Sprite* sprite) { _sprite_hover = sprite; }
    // inline void setSpritePressed(Sprite* sprite) { _sprite_pressed = sprite; }

    inline Sprite* getSpriteNormal() { return _sprite_normal; }
    inline Sprite* getSpriteHover() { return _sprite_hover; }
    inline Sprite* getSpritePress() { return _sprite_press; }

    bool isTrigger();
    inline bool isHover() { return is_hover; }
    inline bool isPress() { return is_press; }
    inline void setTrigger(bool trigger) { is_trigger = trigger; }
    inline void setHover(bool hover) { is_hover = hover; }
    inline void setPress(bool press) { is_press = press; }

    void checkState();
    void checkHover();

    void setScale(float scale = 1.0f);

private:
};

#endif