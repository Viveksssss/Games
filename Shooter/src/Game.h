#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "object.h"

#include <SDL_events.h>
#include <SDL_stdinc.h>
#include <SDL_ttf.h>

class TextRenderer;
class Game {
public:
    TextRenderer* textRenderer;
    static Game& getInstance()
    {
        static Game instance;
        return instance;
    }

    ~Game();
    void init();
    void clean();
    void run();

    void changeScene(Scene* scene);
    void handleEvents(SDL_Event* event);
    void update();

    void render();
    void updateBackground();
    void renderBackground();

    SDL_Window* getWindow() { return this->window; }
    SDL_Renderer* getRenderer() { return this->renderer; }
    int getWindowWidth() { return this->windowWidth; }
    int getWindowHeight() { return this->windowHeight; }
    float getDeltaTime() { return this->deltaTime; }

    TTF_Font* textFont;
    TTF_Font* titleFont;

private:
    Game() = default;
    Game(const Game&) = delete;
    Game(const Game&&) = delete;
    Game& operator=(const Game&) = delete;

    bool isRunning = true;
    Scene* currentScene = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    int windowWidth = 600;
    int windowHeight = 1000;

    float FPS = 120;
    Uint32 frameTime;
    float deltaTime;
    float currentFPS;

    bool isFullScreen = false;

    Background nearStars;
    Background farStars;
};

#endif