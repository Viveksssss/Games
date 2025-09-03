#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H
#include "core/scene.h"


class Player;
class SceneMain : public Scene {
public:
    SceneMain() = default;
    virtual ~SceneMain() = default;
    virtual void init() override;
    virtual void update([[maybe_unused]] float) override;
    virtual void render() override;
    virtual void handleEvents(SDL_Event& event) override;
    virtual void clean() override;

    void render_background();
private:
    Player*player = nullptr;
};

#endif