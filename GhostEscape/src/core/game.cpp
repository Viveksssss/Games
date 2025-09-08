#include "game.h"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <fstream>

#include "../affiliate/sprite.h"
#include "../scene_title.h"

Game& Game::GetInstance()
{
    static Game game;
    return game;
}

void Game::init(const std::string& title, int width, int height)
{
    // 屏蔽系统鼠标
    SDL_HideCursor();
    // 屏幕大小
    this->_screen_size = glm::vec2(width, height);
    // 帧延迟
    this->_frame_delay = 1e9 / this->_FPS;
    // SDL初始化
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_INIT Error: %s", SDL_GetError());
        return;
    }
    // MIX初始化
    if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "MIX_INIT Error: %s", SDL_GetError());
        return;
    }
    Mix_AllocateChannels(32);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_Volume(-1, MIX_MAX_VOLUME / 4);
    if (!Mix_OpenAudio(0, nullptr)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "MIX_OPENAUDIO Error: %s", SDL_GetError());
        return;
    }
    // TTF初始化
    if (!TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_INIT Error: %s", SDL_GetError());
        return;
    }

    // 窗口与渲染器
    SDL_CreateWindowAndRenderer(title.c_str(), width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN, &this->_window, &this->_renderer);
    if (!_window | !_renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_INIT Error: %s", SDL_GetError());
        return;
    }
    _ttf_engine = TTF_CreateRendererTextEngine(_renderer);
    // 逻辑窗口分辨率
    SDL_SetRenderLogicalPresentation(_renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    // 资源类
    _asset_store = new AssetStore(_renderer);
    // 初始化场景
    this->_current_scene = new SceneTitle();
    this->_current_scene->init();
}

void Game::run()
{
    while (isRunning) {
        auto start = SDL_GetTicksNS(); // ns
        if (_next_scene) {
            changeScene(_next_scene);
            _next_scene = nullptr;
        }
        handleEvents();
        update(_dt);
        render();
        auto end = SDL_GetTicksNS(); // ns
        auto elapsed = end - start; // ns
        if (elapsed < this->_frame_delay) {
            SDL_DelayNS((this->_frame_delay - elapsed));
            this->_dt = this->_frame_delay / 1.0e9;
        } else {
            this->_dt = elapsed / 1.0e9;
        }
    }

    clean();
}
void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            isRunning = false;
            break;
        } else {
            this->_current_scene->handleEvents(event);
        }
    }
}

void Game::update(float dt)
{
    _current_scene->update(dt);
    updateMouse();
}
void Game::render()
{
    SDL_RenderClear(this->_renderer);

    _current_scene->render();

    SDL_RenderPresent(this->_renderer);
}

void Game::clean()
{
    if (_current_scene) {
        _current_scene->clean();
        delete _current_scene;
    }
    if (_ttf_engine) {
        TTF_DestroyRendererTextEngine(_ttf_engine);
    }
    if (this->_renderer)
        SDL_DestroyRenderer(this->_renderer);
    if (this->_window)
        SDL_DestroyWindow(this->_window);

    _asset_store->clean();
    delete _asset_store;

    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    TTF_Quit();
}

Scene* Game::getCurrentScene()
{
    return this->_current_scene;
}

void Game::drawGrid(const glm::vec2& top_left, const glm::vec2& bottom_right, int grid_width, SDL_FColor color)
{

    /*
     * 这样绘制是全地图绘制，效率较低
     SDL_SetRenderDrawColorFloat(this->_renderer, color.r, color.g, color.b, color.a);
     //     for (int i = top_left.x; i < bottom_right.x; i += grid_width) {
     //         SDL_RenderLine(this->_renderer, i, top_left.y, i, bottom_right.y);
     //     }
     //     for (int i = top_left.y; i < bottom_right.y; i += grid_width) {
     //         SDL_RenderLine(this->_renderer, top_left.x, i, bottom_right.x, i);
     //     }
     //     SDL_SetRenderDrawColorFloat(this->_renderer, 0, 0, 0, 255);
     *
     */
    // 优化绘制，只绘制可见区域

    // 获取当前渲染视口（可见区域）
    SDL_Rect viewport;
    SDL_GetRenderViewport(this->_renderer, &viewport);

    SDL_SetRenderDrawColorFloat(this->_renderer, color.r, color.g, color.b, color.a);

    // 计算可见区域的网格起始和结束坐标
    int start_x = std::max(static_cast<int>(top_left.x),
        static_cast<int>((viewport.x - top_left.x) / grid_width) * grid_width + static_cast<int>(top_left.x));
    int end_x = std::min(static_cast<int>(bottom_right.x),
        static_cast<int>((viewport.x + viewport.w - top_left.x) / grid_width + 1) * grid_width + static_cast<int>(top_left.x));

    int start_y = std::max(static_cast<int>(top_left.y),
        static_cast<int>((viewport.y - top_left.y) / grid_width) * grid_width + static_cast<int>(top_left.y));
    int end_y = std::min(static_cast<int>(bottom_right.y),
        static_cast<int>((viewport.y + viewport.h - top_left.y) / grid_width + 1) * grid_width + static_cast<int>(top_left.y));

    // 只绘制可见的垂直线
    for (int i = start_x; i <= end_x; i += grid_width) {
        if (i >= top_left.x && i <= bottom_right.x) {
            SDL_RenderLine(this->_renderer, i, top_left.y, i, bottom_right.y);
        }
    }

    // 只绘制可见的水平线
    for (int i = start_y; i <= end_y; i += grid_width) {
        if (i >= top_left.y && i <= bottom_right.y) {
            SDL_RenderLine(this->_renderer, top_left.x, i, bottom_right.x, i);
        }
    }

    SDL_SetRenderDrawColorFloat(this->_renderer, 0, 0, 0, 255);
}

void Game::drawBoundary(const glm::vec2& top_left, const glm::vec2& bottom_right, float line_width, SDL_FColor color)
{
    SDL_SetRenderDrawColorFloat(this->_renderer, color.r, color.g, color.b, color.a);
    for (float i = 0; i < line_width; i += 1) {
        SDL_FRect rect = {
            top_left.x - i,
            top_left.y - i,
            bottom_right.x - top_left.x + 2 * i,
            bottom_right.y - top_left.y + 2 * i
        };
        SDL_RenderRect(this->_renderer, &rect);
    }
    SDL_SetRenderDrawColorFloat(this->_renderer, 0, 0, 0, 1);
}

glm::vec2 Game::getScreenSize()
{
    return this->_screen_size;
}

void Game::renderTexture(const Texture& texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2& mask)
{
    SDL_FRect src = {
        texture.rect.x,
        texture.rect.y + texture.rect.h * (1 - mask.y),
        texture.rect.w * mask.x,
        texture.rect.h * mask.y
    };
    SDL_FRect dst = {
        position.x,
        position.y + size.y * (1 - mask.y),
        size.x * mask.x,
        size.y * mask.y
    };
    SDL_RenderTextureRotated(this->_renderer, texture.texture, &src, &dst, texture.angle, nullptr, texture.is_filp ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void Game::renderBox(const glm::vec2& position, const glm::vec2& size, float alpha)
{
    auto texture = _asset_store->getTexture("assets/UI/circle.png");
    SDL_FRect dst = {
        position.x,
        position.y,
        size.x,
        size.y
    };
    SDL_SetTextureAlphaModFloat(texture, alpha);
    SDL_RenderTexture(this->_renderer, texture, nullptr, &dst);
}

void Game::renderHBar(const glm::vec2& position, const glm::vec2& size, float percentage, SDL_FColor color)
{
    SDL_SetRenderDrawColorFloat(this->_renderer, color.r, color.g, color.b, color.a);
    SDL_FRect bounderRect = {
        position.x,
        position.y,
        size.x,
        size.y
    };
    SDL_FRect fillRect = {
        position.x,
        position.y,
        size.x * percentage,
        size.y
    };
    SDL_RenderRect(this->_renderer, &bounderRect);
    SDL_RenderFillRect(this->_renderer, &fillRect);
    SDL_SetRenderDrawColorFloat(this->_renderer, 0, 0, 0, 1);
}

float Game::randomFloat(float min, float max)
{
    return std::uniform_real_distribution<float>(min, max)(gen);
}

int Game::randomInt(int min, int max)
{
    return std::uniform_int_distribution<int>(min, max)(gen);
}

glm::vec2 Game::randomVec2(const glm::vec2& min, const glm::vec2& max)
{
    return glm::vec2(randomFloat(min.x, max.x), randomFloat(min.y, max.y));
}

glm::ivec2 Game::randomIvec2(const glm::ivec2& min, const glm::ivec2& max)
{
    return glm::ivec2(randomInt(min.x, max.x), randomInt(min.y, max.y));
}

void Game::updateMouse()
{
    _mouse_buttons = SDL_GetMouseState(&_mouse_position.x, &_mouse_position.y);
}

void Game::setScore(int score)
{
    _score = score;
    if (_score > _high_score) {
        _score = _high_score;
    }
}

TTF_Text* Game::createText(const std::string& text, const std::string& font, int size)
{
    auto _font = _asset_store->getFont(font, size);
    return TTF_CreateText(this->_ttf_engine, _font, text.c_str(), 0);
}

bool Game::isMouseInRect(const glm::vec2& top_left, const glm::vec2& bottom_right)
{
    if (_mouse_position.x >= top_left.x && _mouse_position.x <= bottom_right.x && _mouse_position.y >= top_left.y && _mouse_position.y <= bottom_right.y) {
        return true;
    }
    return false;
}

void Game::quit()
{
    isRunning = false;
}

void Game::changeScene(Scene* scene)
{
    if (scene != _current_scene) {
        _current_scene->clean();
        delete _current_scene;
    }
    _current_scene = scene;
    _current_scene->init();
}

std::string Game::loadTextFile(const std::string& path)
{
    std::ifstream file(path);
    std::string line;
    std::string text;
    while(std::getline(file,line)){
        text += line + "\n";
    }  
    return text;
}