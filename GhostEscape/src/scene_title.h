#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "core/scene.h"
#include "screen/hud_button.h"

class HUDText;
class UIMouse;
class SceneTitle : public Scene {
protected:
    SDL_FColor _boundary_color = { 0.5, 0.5, 0.5, 1 };

    HUDButton* _btn_start = nullptr;
    HUDButton* _btn_credits = nullptr;
    HUDButton* _btn_exit = nullptr;
    HUDText* _text_credits = nullptr;
    UIMouse* _mouse = nullptr;

public:
    virtual void init() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual void clean() override;
    virtual bool handleEvents(SDL_Event& event) override;

    virtual void loadData(const std::string& filename) override;

    void checkBtnQuit();
    void checkBtnStart();
    void checkBtnCredits();

private:
    void renderBoundary();
    void updateColor();
};

#endif