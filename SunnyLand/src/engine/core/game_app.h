#pragma once

struct SDL_Window;
struct SDL_Renderer;

namespace engine::core {

class GameApp final {

private:
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    bool _is_running = false;

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
};

};