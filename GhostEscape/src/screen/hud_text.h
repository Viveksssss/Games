#ifndef HUD_TEXT_H
#define HUD_TEXT_H

#include "../affiliate/sprite.h"
#include "../affiliate/text_label.h"

class HUDText : public ObjectScreen {
protected:
    TextLabel* _text;
    Sprite* _sprite;
    glm::vec2 size { 0, 0 };

public:
    static HUDText* create(Object* parent, const std::string& text, const glm::vec2& render_pos, const glm::vec2& size, const std::string& path = "assets/font/VonwaonBitmap-16px.ttf", int font_size = 32, const std::string& bg_path = "assets/UI/Textfield_01.png", Anchor anchor = Anchor::CENTER);

    // getter and setter
    inline TextLabel* getTextLabel() { return _text; }
    inline Sprite* getSprite() { return _sprite; }
    inline void setTextLabel(TextLabel* text) { _text = text; }
    inline void setSprite(Sprite* sprite) { _sprite = sprite; }
    inline void setText(const std::string& text) { _text->setText(text); };
    inline std::string getText() { return _text->getText(); };
    inline glm::vec2 getSize() { return size; }
    void setSize(const glm::vec2& size);
    void setBackground(const std::string& path);
    void adaptBgSize();
};

#endif