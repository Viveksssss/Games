#ifndef TEXT_LABEL_H
#define TEXT_LABEL_H

#include "../core/object_affiliate.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class TextLabel : public ObjectAffiliate {
protected:
    TTF_Text* _ttf_text;
    std::string path;
    int font_size = 32;

public:
    static TextLabel* create(ObjectScreen* parent, const std::string& text, const std::string& path, int size, Anchor anchor = Anchor::CENTER);

    virtual void render() override;
    virtual void clean() override;

    void setFont(const std::string& path, int size);
    inline void setText(const std::string& text) { TTF_SetTextString(_ttf_text, text.c_str(), 0);updateSize(); }
    void setFontPath(const char* path, int size);
    void setTextSize(int size);
    inline std::string getText() { return _ttf_text->text; }
    inline std::string getPath() { return path; }
    inline int getFontSize() { return font_size; }

    void updateSize();
private:
};

#endif