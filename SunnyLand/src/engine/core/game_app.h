#pragma once


#include <memory>
struct SDL_Window;
struct SDL_Renderer;

namespace engine::input {
class InputManager;
}

namespace engine::resource {
class ResourceManager;
}

namespace engine::render {
class Renderer;
class Camera;
}

namespace engine::core {

class Config;
class Time;
class GameApp final {

private:
    SDL_Window* _window = nullptr;
    SDL_Renderer* _sdl_renderer = nullptr;
    bool _is_running = false;

    std::unique_ptr<engine::core::Time> _time;
    std::unique_ptr<engine::resource::ResourceManager> _resource_manager;
    std::unique_ptr<engine::render::Renderer> _renderer;
    std::unique_ptr<engine::render::Camera> _camera;
    std::unique_ptr<engine::core::Config> _config;
    std::unique_ptr<engine::input::InputManager> _input_manager;

public:
    GameApp();
    ~GameApp();

    void run();

    GameApp(const GameApp&) = delete;
    GameApp& operator=(const GameApp&) = delete;
    GameApp(GameApp&&) = delete;
    GameApp& operator=(GameApp&&) = delete;

private:
    [[nodiscard]] bool init();
    void handleEvents();
    void update(float dt);
    void render();
    void close();

    /* 各模块的初始化函数，最后 在init调用 */
    [[nodiscard]] bool initConfig();
    [[nodiscard]] bool initSDL();
    [[nodiscard]] bool initTime();
    [[nodiscard]] bool initResourceManager();
    [[nodiscard]] bool initRenderer();
    [[nodiscard]] bool initCamera();
    [[nodiscard]] bool initInputManager();

    void testResourceManager();
    void testRenderer();
    void testCamera();
    void testInputManager();
    void testGameObject();
};

}