#include "hud_text.h"

void HUDText::setBackground(const std::string& path)
{
    if (_sprite) {
        _sprite->setTexture(path);
    } else {
        _sprite = Sprite::create(this, path);
    }
}

void HUDText::adaptBgSize()
{
    _text->updateSize();
    _sprite->setSize(_text->getSize() + glm::vec2(50, 20));
}
HUDText* HUDText::create(Object* parent, const std::string& text, const glm::vec2& render_pos, const glm::vec2& size, const std::string& path, int font_size, const std::string& bg_path, Anchor anchor)
{
    auto hud_text = new HUDText();
    hud_text->init();
    hud_text->setRenderPosition(render_pos);
    hud_text->setSprite(Sprite::create(hud_text, bg_path, 1.0f, anchor));
    hud_text->setTextLabel(TextLabel::create(hud_text, text, path, font_size, anchor));
    hud_text->setSize(size);
    if (parent) {
        parent->addChild(hud_text);
    }
    return hud_text;
}

void HUDText::setSize(const glm::vec2& size)
{
    this->size = size;
    _sprite->setSize(size);
}
