#include "scene_title.h"
#include "scene_main.h"
#include "screen/hud_button.h"
#include "screen/hud_text.h"
#include <SDL3/SDL_timer.h>

void SceneTitle::init()
{
    Scene::init();
    SDL_ShowCursor();
    game.playMusic("assets/bgm/Spooky music.mp3");

    glm::vec2 size = glm::vec2(game.getScreenSize().x / 2.0f, game.getScreenSize().y / 3.0f);
    HUDText::create(this, "幽 灵 逃 生", game.getScreenSize() / 2.0f - glm::vec2(0, 100), size, "assets/font/VonwaonBitmap-16px.ttf", 64);
    auto score_text = "最高分: " + std::to_string(game.getHighScore());
    HUDText::create(this, score_text, game.getScreenSize() / 2.0f + glm::vec2(0, 150), glm::vec2(500, 50), "assets/font/VonwaonBitmap-16px.ttf", 32);

    // 离开按钮
    _btn_exit = HUDButton::create(this, game.getScreenSize() / 2.0f + glm::vec2(300, 300), "assets/UI/A_Quit1.png", "assets/UI/A_Quit2.png", "assets/UI/A_Quit3.png", 2.0f);
    // 开始按钮
    _btn_start = HUDButton::create(this, game.getScreenSize() / 2.0f + glm::vec2(-300, 300), "assets/UI/A_Start1.png", "assets/UI/A_Start2.png", "assets/UI/A_Start3.png", 2.0f);

    // 得到credits文字
    auto text = game.loadTextFile("assets/credits.txt");
    _text_credits = HUDText::create(this, text, game.getScreenSize() / 2.0f, glm::vec2(300, 300), "assets/font/VonwaonBitmap-16px.ttf", 16);
    _text_credits->adaptBgSize();
    _text_credits->setActive(false);
    _btn_credits = HUDButton::create(this, game.getScreenSize() / 2.0f + glm::vec2(0, 300), "assets/UI/A_Credits1.png", "assets/UI/A_Credits2.png", "assets/UI/A_Credits3.png", 2.0f);
}

void SceneTitle::update(float dt)
{
    Scene::update(dt);
    updateColor();
    checkBtnQuit();
    checkBtnStart();
    checkBtnCredits();
}

void SceneTitle::render()
{
    renderBoundary();
    Scene::render();
}

void SceneTitle::clean()
{
    Scene::clean();
}

bool SceneTitle::handleEvents(SDL_Event& event)
{
    if (Scene::handleEvents(event))
        return true;
    return false;
}

void SceneTitle::checkBtnQuit()
{
    if (_btn_exit->isTrigger()) {
        game.quit();
    }
}

void SceneTitle::checkBtnStart()
{
    if (_btn_start->isTrigger()) {
        auto scene = new SceneMain();
        game.safeChangeScene(scene);
    }
}

void SceneTitle::checkBtnCredits()
{
    if (_btn_credits->isTrigger()) {
        _text_credits->setActive(!_text_credits->isActive());
    }
}

void SceneTitle::renderBoundary()
{
    game.drawBoundary(glm::vec2(30), { game.getScreenSize() - glm::vec2(30) }, 10, _boundary_color);
}

void SceneTitle::updateColor()
{
    _boundary_color.r = 0.5f + std::sinf(SDL_GetTicks() / 1000.0f * 1.9f);
    _boundary_color.g = 0.5f + std::sinf(SDL_GetTicks() / 1000.0f * 1.8f);
    _boundary_color.b = 0.5f + std::sinf(SDL_GetTicks() / 1000.0f * 1.7f);
}
