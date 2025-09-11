#include "game_app.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <spdlog/spdlog.h>

#include "../resource/resource_manager.h"

#include "time.h"

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

    _time->setTargetFPS(240);

    while (_is_running) {
        // float dt = 0.01f;
        _time->update();
        float dt = _time->getDeltaTime();
        handleEvents();
        update(dt);
        render();
        // spdlog::info("delta_time:{}", _time->getDeltaTime());
    }

    close();
}

bool GameApp::init()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::trace("初始化GameApp...");

    if (!initSDL())
        return false;
    if (!initTime())
        return false;
    if (!initResourceManager())
        return false;

    testResourceManager();

    _is_running = true;

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

bool GameApp::initSDL()
{
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
    spdlog::debug("SDL 初始化成功...");

    SDL_RenderPresent(_renderer);
    return true;
}

bool GameApp::initTime()
{
    try {
        _time = std::make_unique<Time>();
    } catch (const std::exception& e) {
        spdlog::error("初始化时间管理失败: {}", e.what());
        return false;
    }
    spdlog::debug("时间管理初始化成功。");
    return true;
}

bool GameApp::initResourceManager()
{
    try {
        _resource_manager = std::make_unique<engine::resource::ResourceManager>(_renderer);
    } catch (const std::exception& e) {
        spdlog::error("初始化资源管理器失败: {}", e.what());
        return false;
    }
    spdlog::debug("资源管理器初始化成功。");
    return true;
}

void GameApp::testResourceManager()
{
    _resource_manager->getTexture("assets/textures/Actors/eagle-attack.png");
    _resource_manager->getFont("assets/fonts/VonwaonBitmap-16px.ttf", 16);
    _resource_manager->getSound("assets/audio/button_click.wav");

    _resource_manager->unloadTexture("assets/textures/Actors/eagle-attack.png");
    _resource_manager->unloadFont("assets/fonts/VonwaonBitmap-16px.ttf", 16);
    _resource_manager->unloadSound("assets/audio/button_click.wav");
}

GameApp::GameApp() = default;
} // namespace engine::core