#include "ui_mouse.h"
void UIMouse::update(float dt)
{
    ObjectScreen::update(dt);
    timer += dt;
    if (timer < switch_time) {
        _sprite1->setActive(true);
        _sprite2->setActive(false);
    } else if (timer < 2 * switch_time) {
        _sprite1->setActive(false);
        _sprite2->setActive(true);
    } else {
        timer = 0;
    }
    setRenderPosition(game.getMousePosition());
}

UIMouse* UIMouse::create(Object* parent, const std::string& path1, const std::string& path2, float scale, Anchor anchor)
{
    auto ui_mouse = new UIMouse();
    ui_mouse->init();
    ui_mouse->_sprite1 = Sprite::create(ui_mouse, path1, scale, anchor);
    ui_mouse->_sprite2 = Sprite::create(ui_mouse, path2, scale, anchor);
    if (parent)
        parent->addChild(ui_mouse);
    return ui_mouse;
}
