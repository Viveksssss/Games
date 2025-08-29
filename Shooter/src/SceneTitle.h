#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "Scene.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class Game;

class SceneTitle : public Scene {
public:
    SceneTitle();
    void init() override;
    void clean() override;
    void update() override;
    void render() override;
    void handleEvents(SDL_Event* event) override;

private:
    Mix_Music* music;
    float timer = 0.0f;
};

#endif