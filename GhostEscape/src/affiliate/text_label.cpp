#include "text_label.h"
#include <SDL3_ttf/SDL_ttf.h>

TextLabel* TextLabel::create(ObjectScreen* parent, const std::string& text, const std::string& path, int size, Anchor anchor)
{
    auto text_label = new TextLabel();
    text_label->init();
    text_label->setFont(path, size);
    text_label->setText(text);
    text_label->setOffsetByAnchor(anchor);
    text_label->updateSize();
    if (parent) {
        parent->addChild(text_label);
        text_label->setParent(parent);
    }
    return text_label;
}

void TextLabel::setFontPath(const char* path, int size)
{
    this->path = path;
    auto font = game.getAssetStore()->getFont(path, size);
    if (font) {
        TTF_SetTextFont(this->_ttf_text, font);
    }
}
void TextLabel::setTextSize(int size)
{
    font_size = size;
    auto font = game.getAssetStore()->getFont(path, size);
    if (font) {
        TTF_SetTextFont(this->_ttf_text, font);
    }
}

void TextLabel::updateSize()
{
    int w, h;
    TTF_GetTextSize(_ttf_text, &w, &h);
    setSize(glm::vec2(w, h));
}

void TextLabel::render()
{
    ObjectAffiliate::render();
    auto pos = _parent->getRenderPosition() + _offset;
    if(!_ttf_text){
        SDL_Log("asdasdsadsad");
    }
    if (!_ttf_text) return;
    TTF_DrawRendererText(_ttf_text, pos.x, pos.y);
}

void TextLabel::setFont(const std::string& path, int size)
{
    this->font_size = size;
    this->path = path;
    auto font = game.getAssetStore()->getFont(path, size);
    if (!_ttf_text) {
        _ttf_text = game.createText("", path, font_size);
    }
    TTF_SetTextFont(_ttf_text, font);
}

void TextLabel::clean()
{
    TTF_DestroyText(_ttf_text);
}