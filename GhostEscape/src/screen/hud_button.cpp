#include "hud_button.h"

void HUDButton::checkState()
{
    if (!is_press && !is_hover) {
        _sprite_normal->setActive(true);
        _sprite_hover->setActive(false);
        _sprite_press->setActive(false);
    } else if (!is_press && is_hover) {
        _sprite_normal->setActive(false);
        _sprite_hover->setActive(true);
        _sprite_press->setActive(false);
    } else {
        _sprite_normal->setActive(false);
        _sprite_hover->setActive(false);
        _sprite_press->setActive(true);
    }
}

void HUDButton::checkHover()
{
    bool new_hover;
    auto pos = getRenderPosition() + _sprite_normal->getOffset();
    auto size = _sprite_normal->getSize();
    if (game.isMouseInRect(pos, pos + size)) {
        new_hover = true;
    } else {
        new_hover = false;
    }
    if (new_hover != is_hover) {
        is_hover = new_hover;
        if (is_hover && !is_press)
            game.playSound("assets/sound/UI_button12.wav");
    }
}

void HUDButton::setScale(float scale)
{
    _sprite_normal->setScale(scale);
    _sprite_hover->setScale(scale);
    _sprite_press->setScale(scale);
}

bool HUDButton::handleEvents(SDL_Event& event)
{
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            if (is_hover) {
                is_press = true;
                game.playSound("assets/sound/UI_button08.wav");
                return true;
            }
        }
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            is_press = false;
            if (is_hover) {
                is_trigger = true;
                return true;
            }
        }
    }
    return false;
}

void HUDButton::update(float dt)
{
    ObjectScreen::update(dt);
    checkHover();
    checkState();
}

bool HUDButton::isTrigger()
{
    if (is_trigger) {
        is_trigger = false;
        is_hover = false;
        is_press = false;
        return true;
    }
    return false;
}

HUDButton* HUDButton::create(Object* parent, const glm::vec2& pos, const std::string& file1, const std::string& file2, const std::string& file3, float scale, Anchor anchor)
{
    auto button = new HUDButton();
    button->init();
    button->setRenderPosition(pos);
    button->_sprite_hover = Sprite::create(button, file1, scale, anchor);
    button->_sprite_normal = Sprite::create(button, file2, scale, anchor);
    button->_sprite_press = Sprite::create(button, file3, scale, anchor);
    button->_sprite_hover->setActive(false);
    button->_sprite_press->setActive(false);
    if (parent) {
        parent->addChild(button);
    }
    return button;
}
