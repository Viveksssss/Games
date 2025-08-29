#include "Game.h"
#include "SceneEnd.h"
#include "SceneMain.h"
#include "SceneTitle.h"

#include <SDL.h>
#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>

#include "TextRender.h"
#include "tool.h"

/**
 * Game类的析构函数
 * 用于在Game对象生命周期结束时进行清理工作
 */
Game::~Game()
{
    if (SceneEnd* scene = dynamic_cast<SceneEnd*>(currentScene)) {
        scene->saveFileData();
    }
    clean();
}
/**
 * 游戏主运行函数
 * 该函数负责游戏的主要循环和逻辑控制
 * 当游戏运行时，会持续执行直到游戏结束
 */
void Game::run()
{
    while (isRunning) {
        float frameStart = SDL_GetTicks();

        SDL_Event event;
        handleEvents(&event);
        update();
        render();

        float frameEnd = SDL_GetTicks();
        float diff = frameEnd - frameStart;
        if (diff < frameTime) {
            deltaTime = frameTime / 1000.0f;
            SDL_Delay(frameTime - diff);
        } else {
            currentFPS = 1000.0f / diff;
            deltaTime = diff / 1000.0f;
        }
    }
}

/**
 * 游戏初始化函数
 * 该函数用于初始化游戏的基本设置和状态
 * 在游戏开始前调用，确保游戏环境正确配置
 */
void Game::init()
{

    textRenderer = new TextRenderer;
    frameTime = 1000 / FPS;
    currentFPS = FPS;
    if (!textRenderer->init()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Error:%s\n", SDL_GetError());
        isRunning = false;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Error:%s\n", SDL_GetError());
        isRunning = false;
    }
    window = SDL_CreateWindow("Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_VULKAN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d:SDL_Error::%s\n", __LINE__, SDL_GetError());
        isRunning = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d:SDL_Error::%s\n", __LINE__, SDL_GetError());
        isRunning = false;
    }
    // 设置逻辑分辨率
    SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);

    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d:SDL_Error::%s\n", __LINE__, SDL_GetError());
        isRunning = false;
    }

    if (TTF_Init() != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d:SDL_Error::%s\n", __LINE__, SDL_GetError());
        isRunning = false;
    }
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d:SDL_Error::%s\n", __LINE__, SDL_GetError());
        isRunning = false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d:SDL_Error::%s\n", __LINE__, SDL_GetError());
        isRunning = false;
    }
    if ((textFont = TTF_OpenFont(getPath("assets/font/VonwaonBitmap-12px.ttf").c_str(), 32)) == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d:SDL_Error::%s\n", __LINE__, SDL_GetError());
        isRunning = false;
    }
    if ((titleFont = TTF_OpenFont(getPath("assets/font/VonwaonBitmap-16px.ttf").c_str(), 64)) == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d:SDL_Error::%s\n", __LINE__, SDL_GetError());
        isRunning = false;
    }

    nearStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-A.png");
    SDL_QueryTexture(nearStars.texture, nullptr, nullptr, &nearStars.width, &nearStars.height);

    farStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-B.png");
    SDL_QueryTexture(farStars.texture, nullptr, nullptr, &farStars.width, &farStars.height);

    currentScene = new SceneTitle;
    currentScene->init();
}

/**
 * 游戏清理函数
 * 该函数用于在游戏结束时进行资源清理和内存释放工作
 * 确保所有动态分配的资源都被正确释放，避免内存泄漏
 */
void Game::clean()
{

    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
    }

    Mix_CloseAudio();
    Mix_Quit();

    TTF_Quit();

    if (nearStars.texture != nullptr)
        SDL_DestroyTexture(nearStars.texture);
    if (farStars.texture != nullptr)
        SDL_DestroyTexture(farStars.texture);
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

/**
 * @brief 切换游戏场景的函数
 * @param scene 指向新场景的指针，用于替换当前场景
 */
void Game::changeScene(Scene* scene)
{
    if (currentScene != nullptr) {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->init();
}

void Game::update()
{
    // 更新背景
    updateBackground();
    // 更新场景
    currentScene->update();
}

void Game::render()
{
    SDL_RenderClear(renderer);
    // 帧率显示
    std::string fpsText = "FPS:" + std::to_string(currentFPS);
    textRenderer->renderText(renderer, fpsText, 5, 0);
    // 背景渲染
    renderBackground();
    // 场景渲染
    currentScene->render();
    SDL_RenderPresent(renderer);
}

void Game::handleEvents(SDL_Event* event)
{
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            isRunning = false;
        }
        if (event->key.type == SDL_KEYDOWN) {
            if (event->key.keysym.sym == SDLK_ESCAPE)
                isRunning = false;
            if (event->key.keysym.sym == SDLK_F4) {
                isFullScreen = !isFullScreen;
                if (isFullScreen) {
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                } else {
                    SDL_SetWindowFullscreen(window, 0);
                }
            }
        }
        currentScene->handleEvents(event);
    }
}

void Game::updateBackground()
{
    nearStars.offset += deltaTime * nearStars.speed * 200;
    if (nearStars.offset >= 0) {
        nearStars.offset -= nearStars.height;
    }
    farStars.offset += deltaTime * farStars.speed * 30;
    if (farStars.offset >= 0) {
        farStars.offset -= farStars.height;
    }
}

void Game::renderBackground()
{
    int posY = static_cast<int>(nearStars.offset);
    for (; posY < windowHeight; posY += nearStars.height) {
        for (int posX = 0; posX < windowWidth; posX += nearStars.width) {
            SDL_Rect dstRect = { posX, posY, nearStars.width, nearStars.height };
            SDL_RenderCopy(renderer, nearStars.texture, nullptr, &dstRect);
        }
    }
    posY = static_cast<int>(farStars.offset);
    for (; posY < windowHeight; posY += farStars.height) {
        for (int posX = 0; posX < windowWidth; posX += farStars.width) {
            SDL_Rect dstRect = { posX, posY, farStars.width, farStars.height };
            SDL_RenderCopy(renderer, farStars.texture, nullptr, &dstRect);
        }
    }
}