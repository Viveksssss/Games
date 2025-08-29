#ifndef SCENE_H
#define SCENE_H

#include <SDL.h>
#include <SDL_events.h>
class Game;

class Scene {
public:
    Scene();
    virtual ~Scene() = default;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual void handleEvents(SDL_Event* event) = 0;

protected:
    Game& game;
};

#endif