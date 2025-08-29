#ifndef TEXTRENDER_H
#define TEXTRENDER_H

#include <SDL_ttf.h> // 包含 TTF 头文件
#include <string>
class Game;


class TextRenderer {
private:
    Game* game;
    TTF_Font* font;
    SDL_Color textColor;

public:
    TextRenderer();
    TextRenderer(TTF_Font* font);

    ~TextRenderer();

    bool init();

    SDL_Point renderText(SDL_Renderer* renderer, const std::string& text, int x, int y);
    SDL_Point renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int y);
    SDL_Point renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y);


};

#endif