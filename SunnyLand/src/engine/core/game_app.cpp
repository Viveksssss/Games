#include "game_app.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <spdlog/spdlog.h>

#include "../render/camera.h"
#include "../render/renderer.h"
#include "../resource/resource_manager.h"
#include "config.h"

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

    if (!initConfig())
        return false;
    if (!initSDL())
        return false;
    if (!initTime())
        return false;
    if (!initResourceManager())
        return false;
    if (!initRenderer())
        return false;
    if (!initCamera())
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

void GameApp::update(float)
{
    testCamera();
}

void GameApp::render()
{
    _renderer->clearScreen();

    testRenderer();

    _renderer->present();
}

void GameApp::close()
{
    spdlog::info("关闭GameApp...");
    if (_renderer != nullptr) {
        SDL_DestroyRenderer(_sdl_renderer);
        _renderer = nullptr;
    }
    if (_window != nullptr) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
    SDL_Quit();
    _is_running = false;
}

bool GameApp::initConfig()
{
    try {
        _config = std::make_unique<engine::core::Config>("assets/config.json");
    } catch (const std::exception& e) {
        spdlog::error("初始化配置文件失败: {}", e.what());
        return false;
    }
    return true;
}

bool GameApp::initSDL()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        spdlog::error("SDL初始化失败,错误:{}...", SDL_GetError());
        return false;
    }

    _window = SDL_CreateWindow("SunnyLand", _config->_window_width, _config->_window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
    if (_window == nullptr) {
        spdlog::error("无法创建窗口!错误:{}...", SDL_GetError());
        return false;
    }

    _sdl_renderer = SDL_CreateRenderer(_window, nullptr);
    if (_sdl_renderer == nullptr) {
        spdlog::error("无法创建渲染器!错误:{}...", SDL_GetError());
        return false;
    }

    int vsync = _config->_vsync_enabled ? SDL_RENDERER_VSYNC_ADAPTIVE : SDL_RENDERER_VSYNC_DISABLED;
    SDL_SetRenderVSync(_sdl_renderer, vsync);
    spdlog::info("VSync:{}", vsync);

    SDL_SetRenderLogicalPresentation(_sdl_renderer, _config->_window_width/2, _config->_window_height/2 , SDL_LOGICAL_PRESENTATION_LETTERBOX);

    spdlog::debug("SDL 初始化成功...");

    // SDL_RenderPresent(_sdl_renderer);
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
    _time->setTargetFPS(_config->_target_fps);
    spdlog::debug("时间管理初始化成功。");
    return true;
}

bool GameApp::initResourceManager()
{
    try {
        _resource_manager = std::make_unique<engine::resource::ResourceManager>(_sdl_renderer);
    } catch (const std::exception& e) {
        spdlog::error("初始化资源管理器失败: {}", e.what());
        return false;
    }
    spdlog::debug("资源管理器初始化成功。");
    return true;
}

bool GameApp::initRenderer()
{
    try {
        _renderer = std::make_unique<engine::render::Renderer>(_sdl_renderer, _resource_manager.get());
    } catch (const std::exception& e) {
        spdlog::error("初始化渲染器失败: {}", e.what());
        return false;
    }

    spdlog::debug("渲染器初始化成功。");
    return true;
}

bool GameApp::initCamera()
{
    try {
        _camera = std::make_unique<engine::render::Camera>(glm::vec2(_config->_window_width/2, _config->_window_height/2));
    } catch (const std::exception& e) {
        spdlog::error("初始化摄像机失败: {}", e.what());
        return false;
    }
    spdlog::debug("摄像机初始化成功。");
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

void GameApp::testRenderer()
{
    engine::render::Sprite sprite_world("assets/textures/Actors/frog.png");
    engine::render::Sprite sprite_ui("assets/textures/UI/buttons/Start1.png");
    engine::render::Sprite sprite_parallax("assets/textures/Layers/back.png");

    static float rotation = 0.0f;
    rotation += 0.1f;

    // 注意渲染顺序
    _renderer->drawParallax(*_camera, sprite_parallax, glm::vec2(0, 0), glm::vec2(0.5f, 0.5f), glm::bvec2(true, false));
    _renderer->drawSprite(*_camera, sprite_world, glm::vec2(200, 200), glm::vec2(1.0f, 1.0f), rotation);
    _renderer->drawUISprite(sprite_ui, glm::vec2(100, 100));
}

void GameApp::testCamera()
{
    auto key_state = SDL_GetKeyboardState(nullptr);
    if (key_state[SDL_SCANCODE_UP])
        _camera->move(glm::vec2(0, -1));
    if (key_state[SDL_SCANCODE_DOWN])
        _camera->move(glm::vec2(0, 1));
    if (key_state[SDL_SCANCODE_LEFT])
        _camera->move(glm::vec2(-1, 0));
    if (key_state[SDL_SCANCODE_RIGHT])
        _camera->move(glm::vec2(1, 0));
}

GameApp::GameApp() = default;
} // namespace engine::core