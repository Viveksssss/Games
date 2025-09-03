#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>

#include "asset_store.h"
#include "debug.h"
#include <string>

struct Texture;

class Scene;
class Game {
private:
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    glm::vec2 _screen_size = glm::vec2(0);
    bool isRunning = true;
    Scene* _current_scene = nullptr;
    AssetStore* _asset_store;

    // 秒间隔：s
    float _dt = 0.0f;
    // 帧延迟：ns
    float _frame_delay = 0.0f;
    // 帧率：fps
    float _FPS = 240.0f;

    Game() { };
    Game(Game&) { };
    Game& operator=(const Game&) = delete;

public:
    static Game& GetInstance();

    void run();
    void init(const std::string& title, int width, int height);
    void update([[maybe_unused]] float);
    void handleEvents();
    void render();
    void clean();

    Scene* getCurrentScene();

    // 绘制网格
    void drawGrid(const glm::vec2& top_left, const glm::vec2& bottom_right, int grid_width, SDL_FColor color);
    // 绘制边框
    void drawBoundary(const glm::vec2& top_left, const glm::vec2& bottom_right, float line_width, SDL_FColor color);
    // 获取窗口大小
    glm::vec2 getScreenSize();

    // 获取资源类
    inline AssetStore* getAssetStore() { return _asset_store; };
    // 渲染函数
    void renderTexture(const Texture& texture, const glm::vec2& position, const glm::vec2& size);
    // 测试渲染碰撞盒
    void renderBox(const glm::vec2& position, const glm::vec2& size, float alpha);
};

#endif