#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H
#include "core/scene.h"
#include "screen/hud_stats.h"
#include "screen/hud_text.h"

class Player;
class Spawner;
class UIMouse;
class HUDStats;
class HUDText;
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
    void update_score();

private:
    Player* player = nullptr;
    Spawner* spawner = nullptr;
    UIMouse* ui_mouse = nullptr;
    HUDStats* hud_stats = nullptr;
    HUDText* hud_text = nullptr;
};

#endif