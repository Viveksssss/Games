#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>

#include <string>

class Game {
private:
    Game() { };
    Game(Game&) { };
    Game& operator=(const Game&) { };

    glm::vec2 _screen_size = glm::vec2(0);

public:
    static Game& GetInstance();

    void run();
    void init(const std::string& title, int width, int height);
    void update(float dt);
    void handleEvents();
    void render();
    void clean();
};

#endif