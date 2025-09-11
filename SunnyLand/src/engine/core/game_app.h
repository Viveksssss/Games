#pragma once

#include <memory>

struct SDL_Window;
struct SDL_Renderer;

namespace engine::resource {
class ResourceManager;
}

namespace engine::core {

class Time;
class GameApp final {

private:
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    bool _is_running = false;

    std::unique_ptr<engine::core::Time> _time;
    std::unique_ptr<engine::resource::ResourceManager> _resource_manager;

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
    bool initSDL();
    bool initTime();
    bool initResourceManager();

    void testResourceManager();
};

}