#pragma once

#include "sprite.h"
#include <glm/glm.hpp>
#include <optional>


struct SDL_Renderer;
struct SDL_FRect;

namespace engine::resource {
class ResourceManager;
}

namespace engine::render {
class Camera;

class Renderer final {

private:
    /* 指向但不拥有无需管理内存 */
    SDL_Renderer* _renderer = nullptr;
    engine::resource::ResourceManager* _resource_manager = nullptr;

public:
    /**
     * @brief 构造函数
     * @param renderer 渲染器
     * @param resource_anager  资源管理器
     */
    Renderer(SDL_Renderer* renderer, engine::resource::ResourceManager* resource_anager);

    /**
     * @brief 绘制一个精灵
     * @param camera 摄像机
     * @param sprite  精灵
     * @param position 位置
     * @param scale 缩放
     * @param angle 角度
     */
    void drawSprite(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scale = { 1.0f, 1.0f }, double angle = 0.0f);

    /**
     * @brief 绘制视察滚动背景
     * @param camera
     * @param sprite
     * @param position
     * @param parallax
     * @param scale
     * @param angle
     */
    void drawParallax(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scroll_factor, const glm::bvec2& repeat = { true, true }, const glm::vec2& scale = { 1.0f, 1.0f });

    /**
     * @brief 绘制一个用于UI的Sprite对象
     * @param position
     * @param size
     */
    void drawUISprite(const Sprite& sprite, const glm::vec2& position, const std::optional<glm::vec2>& size = std::nullopt);

    /**
     * @brief 更新屏幕，封装了SDL_RenderPresent
     */
    void present();

    /**
     * @brief 清屏，包装 SDL_RenderClear 函数
     */
    void clearScreen();

    /**
     * @brief 设置渲染颜色
     * @param r
     * @param g
     * @param b
     * @param a
     */
    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

    /**
     * @brief 设置渲染颜色
     * @param r
     * @param g
     * @param b
     * @param a
     */
    void setDrawColorFloat(float r, float g, float b, float a = 1.0f);

    SDL_Renderer* getSDLRenderere() const;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

private:
    std::optional<SDL_FRect> getSpriteSrcRect(const Sprite& sprite) const;
    bool isRectInViewport(const Camera& camera, const SDL_FRect& rect);
};

}