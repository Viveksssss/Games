#include "game_app.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <spdlog/spdlog.h>

namespace engine::core {
GameApp::~GameApp()
{
    if (_is_running) {
        spdlog::warn("GameApp 未显示关闭，现在关闭...");
        close();
    }
}

void GameApp::run()
{
    if (!init()) {
        spdlog::error("初始化失败，无法进行游戏...");
        return;
    }

    while (_is_running) {
        float dt = 0.01f;
        handleEvents();
        update(dt);
        render();
    }

    close();
}

bool GameApp::init()
{
    spdlog::trace("初始化GameApp...");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        spdlog::error("SDL初始化失败,错误:{}...", SDL_GetError());
        return false;
    }

    _window = SDL_CreateWindow("SunnyLand", 1920, 1080, SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
    if (_window == nullptr) {
        spdlog::error("无法创建窗口!错误:{}...", SDL_GetError());
        return false;
    }

    _renderer = SDL_CreateRenderer(_window, nullptr);
    if (_renderer == nullptr) {
        spdlog::error("无法创建渲染器!错误:{}...", SDL_GetError());
        return false;
    }

    _is_running = true;
    SDL_RenderPresent(_renderer);

    return true;
}

void GameApp::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            _is_running = false;
        }
    }
}

void GameApp::update(float dt)
{
}

void GameApp::render()
{
}

void GameApp::close()
{
    spdlog::info("关闭GameApp...");
    if (_renderer != nullptr) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if (_window != nullptr) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
    SDL_Quit();
    _is_running = false;
}

GameApp::GameApp() = default;

}; // namespace engine::core