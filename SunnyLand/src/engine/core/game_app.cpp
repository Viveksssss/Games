#include "game_app.h"

#include "../../game/scene/game_scene.h"
#include "../component/sprite_component.h"
#include "../input/input_manager.h"
#include "../object/game_object.h"
#include "../render/camera.h"
#include "../render/renderer.h"
#include "../resource/resource_manager.h"
#include "../scene/scene_manager.h"
#include "context.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <spdlog/spdlog.h>

#include "config.h"

#include "time.h"

namespace engine::core {
engine::object::GameObject obj("test_game_object");
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
        _input_manager->update();

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
    if (!initInputManager())
        return false;
    if (!initContext())
        return false;
    if (!initSceneManager())
        return false;

    auto scene = std::make_unique<game::scene::GameScene>("GameScene", *_context, *_scene_manager);
    _scene_manager->requestPushScene(std::move(scene));

    _is_running = true;

    return true;
}

void GameApp::handleEvents()
{
    if (_input_manager->shouldQuit()) {
        spdlog::debug("GameApp 接收到退出事件，准备关闭...");
        _is_running = false;
        return;
    }
    _scene_manager->handleInput();
}

void GameApp::update(float dt)
{
    _scene_manager->update(dt);
}

void GameApp::render()
{
    _renderer->clearScreen();

    _scene_manager->render();

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

    SDL_SetRenderLogicalPresentation(_sdl_renderer, _config->_window_width / 2, _config->_window_height / 2, SDL_LOGICAL_PRESENTATION_LETTERBOX);

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
        _camera = std::make_unique<engine::render::Camera>(glm::vec2(_config->_window_width / 2, _config->_window_height / 2));
    } catch (const std::exception& e) {
        spdlog::error("初始化摄像机失败: {}", e.what());
        return false;
    }
    spdlog::debug("摄像机初始化成功。");
    return true;
}

bool GameApp::initInputManager()
{
    try {
        _input_manager = std::make_unique<engine::input::InputManager>(_sdl_renderer, _config.get());
    } catch (const std::exception& e) {
        spdlog::error("初始化输入管理器失败: {}", e.what());
        return false;
    }
    spdlog::trace("输入管理器初始化成功。");
    return true;
}

bool GameApp::initContext()
{
    try {
        _context = std::make_unique<engine::core::Context>(*_input_manager, *_renderer, *_camera, *_resource_manager);
    } catch (const std::exception& e) {
        spdlog::error("初始化上下文失败: {}", e.what());
        return false;
    }
    return true;
}

bool GameApp::initSceneManager()
{
    try {
        _scene_manager = std::make_unique<engine::scene::SceneManager>(*_context);
    } catch (const std::exception& e) {
        spdlog::error("初始化场景管理器失败: {}", e.what());
        return false;
    }
    return true;
}

GameApp::GameApp() = default;
} // namespace engine::core