#include "hud_skill.h"
#include "../affiliate/sprite.h"
#include <SDL3/SDL_render.h>
void HUDSkill::setPercentage(float percentage)
{
    percentage = glm::clamp(percentage, 0.0f, 1.0f);
    _percentage = percentage;
    if (_icon) {
        _icon->setPercentage({ 1.0f, percentage });
    }
}

HUDSkill* HUDSkill::create(Object* parent, const std::string& path, const glm::vec2& pos, float scale, Anchor acnhor)
{
    auto hud_skill = new HUDSkill();
    hud_skill->init();
    hud_skill->_icon = Sprite::create(hud_skill, path, scale, acnhor);
    hud_skill->setRenderPosition(pos);
    if (parent) {
        parent->addChild(hud_skill);
    }
    return hud_skill;
}

void HUDSkill::render()
{
    SDL_SetTextureColorModFloat(_icon->getTexture().texture, 0.3, 0.3, 0.3);
    auto pos = getRenderPosition() + _icon->getOffset();
    game.renderTexture(_icon->getTexture(), pos, _icon->getSize());
    SDL_SetTextureColorModFloat(_icon->getTexture().texture, 1.0f, 1.0f, 1.0f);

    ObjectScreen::render();
}
