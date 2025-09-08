#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>
#include <random>

#include "../debug.h"
#include "asset_store.h"
#include <string>

struct Texture;

class Scene;
class Game {
private:
    // 游戏窗口
    SDL_Window* _window = nullptr;
    // 渲染器
    SDL_Renderer* _renderer = nullptr;
    // 字体引擎
    TTF_TextEngine* _ttf_engine = nullptr;
    // 游戏是否运行
    bool isRunning = true;
    // 屏幕大小
    glm::vec2 _screen_size = glm::vec2(0);
    // 当前场景
    Scene* _current_scene = nullptr;
    // 下一场景
    Scene* _next_scene = nullptr;
    // 资源器
    AssetStore* _asset_store;
    // 鼠标位置
    glm::vec2 _mouse_position = glm::vec2(0);
    // 鼠标按键状态
    SDL_MouseButtonFlags _mouse_buttons = 0;

    int _score = 0;
    int _high_score = 0;

    // 秒间隔：s
    float _dt = 0.0f;
    // 帧延迟：ns
    float _frame_delay = 0.0f;
    // 帧率：fps
    float _FPS = 240.0f;
    // 随机数
    std::mt19937 gen = std::mt19937(std::random_device {}());

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

    virtual Scene* getCurrentScene();

    // 绘制网格
    void drawGrid(const glm::vec2& top_left, const glm::vec2& bottom_right, int grid_width, SDL_FColor color);
    // 绘制边框
    void drawBoundary(const glm::vec2& top_left, const glm::vec2& bottom_right, float line_width, SDL_FColor color);
    // 获取窗口大小
    glm::vec2 getScreenSize();

    // 获取资源类
    inline AssetStore* getAssetStore() { return _asset_store; };
    // 文字函数
    TTF_Text* createText(const std::string& text, const std::string& font, int size = 32);
    // 渲染函数
    void renderTexture(const Texture& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2& mask = { 1.0f, 1.0f });
    // 测试渲染碰撞盒
    void renderBox(const glm::vec2& position, const glm::vec2& size, float alpha);
    // 渲染bar
    void renderHBar(const glm::vec2& position, const glm::vec2& size, float percentage, SDL_FColor color);
    // 生成随机数
    float randomFloat(float min, float max);
    int randomInt(int min, int max);
    glm::vec2 randomVec2(const glm::vec2& min, const glm::vec2& max);
    glm::ivec2 randomIvec2(const glm::ivec2& min, const glm::ivec2& max);

    // 播放音频
    // Mix_Music
    inline void playMusic(const std::string& path, bool loop = true) { Mix_PlayMusic(_asset_store->getMusic(path), loop ? -1 : 0); }
    // Mix_Chunk
    inline void playSound(const std::string& path) { Mix_PlayChannel(-1, _asset_store->getSound(path), 0); }
    inline void stopMusic() { Mix_HaltMusic(); }
    inline void stopSound() { Mix_HaltChannel(-1); }
    inline void pauseMusic() { Mix_PauseMusic(); }
    inline void pauseSound() { Mix_Pause(-1); }
    inline void resumeMusic() { Mix_ResumeMusic(); }
    inline void resumeSound() { Mix_Resume(-1); }

    // 获取鼠标位置
    inline glm::vec2 getMousePosition() { return _mouse_position; }
    // 获取鼠标按键状态
    inline SDL_MouseButtonFlags getMouseButton() { return _mouse_buttons; }
    // 设置鼠标位置
    inline void setMousePosition(const glm::vec2& pos) { _mouse_position = pos; }
    // 设置鼠标按键状态
    inline void setMouseButton(SDL_MouseButtonFlags buttons) { _mouse_buttons = buttons; }
    // 鼠标更新
    void updateMouse();
    // 退出游戏
    void quit();
    // 切换场景
    void changeScene(Scene* scene);
    void safeChangeScene(Scene* scene) { _next_scene = scene; }

    void setScore(int score);
    int getScore() { return _score; }
    int getHighScore() { return _high_score; }
    void setHighScore(int high_score) { _high_score = high_score; }
    void addScore(int score) { _score += score; }

    bool isMouseInRect(const glm::vec2& top_left, const glm::vec2& bottom_right);

    std::string loadTextFile(const std::string& path);
};
#endif