#include "TextRender.h"
#include "Game.h"
#include "tool.h"
#include <iostream>

TextRenderer::TextRenderer()
    : font(nullptr)
{
}

TextRenderer::TextRenderer(TTF_Font* font)
    : font(font)

{
}

TextRenderer::~TextRenderer()
{
    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
}

bool TextRenderer::init()
{
    game = &Game::getInstance();
    // 初始化 SDL_ttf
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // 加载字体文件
    font = TTF_OpenFont(getPath("assets/font/VonwaonBitmap-12px.ttf").c_str(), 20); // 字体路径和大小
    if (!font) {
        std::cout << "Failed to load font! Error: " << TTF_GetError() << std::endl;
        return false;
    }

    textColor = { 255, 255, 255, 255 }; // 白色
    return true;
}

SDL_Point TextRenderer::renderText(SDL_Renderer* renderer, const std::string& text, int x, int y)
{
    // 创建文字表面
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, text.c_str(), textColor);
    if (!textSurface) {
        std::cout << "Unable to render text surface! Error: " << TTF_GetError() << std::endl;
        return { 0, 0 };
    }

    // 创建纹理
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cout << "Unable to create texture from text! Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return { 0, 0 };
    }

    // 设置渲染位置
    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };

    // 渲染文字
    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

    // 清理
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);

    return SDL_Point { x + textSurface->w, y };
}

SDL_Point TextRenderer::renderText(SDL_Renderer* renderer, TTF_Font* _font, const std::string& text, int y)
{
    // 创建文字表面
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(_font, text.c_str(), textColor);
    if (!textSurface) {
        std::cout << "Unable to render text surface! Error: " << TTF_GetError() << std::endl;
        return { 0, 0 };
    }

    // 创建纹理
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cout << "Unable to create texture from text! Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return { 0, 0 };
    }

    // 设置渲染位置
    SDL_Rect renderQuad = { game->getWindowWidth() / 2 - textSurface->w / 2, y, textSurface->w, textSurface->h };

    // 渲染文字
    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

    // 清理
    SDL_DestroyTexture(textTexture);
    return SDL_Point { game->getWindowWidth() / 2 + textSurface->w / 2, y };
}

SDL_Point TextRenderer::renderText(SDL_Renderer* renderer, TTF_Font* _font, const std::string& text, int x, int y)
{
    // 创建文字表面
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(_font, text.c_str(), textColor);
    if (!textSurface) {
        std::cout << "Unable to render text surface! Error: " << TTF_GetError() << std::endl;
        return { 0, 0 };
    }

    // 创建纹理
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cout << "Unable to create texture from text! Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return { 0, 0 };
    }

    // 设置渲染位置
    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };

    // 渲染文字
    SDL_RenderCopy(game->getRenderer(), textTexture, nullptr, &renderQuad);

    // 清理
    SDL_DestroyTexture(textTexture);

    return SDL_Point { x + textSurface->w, y };
}
