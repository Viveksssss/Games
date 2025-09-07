#ifndef HUD_SKILL_H
#define HUD_SKILL_H

#include "../core/object_screen.h"

class Sprite;
class HUDSkill : public ObjectScreen {
protected:
    Sprite* _icon = nullptr;
    float _percentage = 1.0f;

public:
    static HUDSkill* create(Object* parent, const std::string& path, const glm::vec2& pos, float scale = 1.0f, Anchor acnhor = Anchor::CENTER);
    virtual void render() override;

    inline Sprite* getIcon() const { return _icon; }
    inline float getPercentage() const { return _percentage; }
    void setPercentage(float percentage);
    void setIcon(Sprite* icon) { _icon = icon; };
};

#endif