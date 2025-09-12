#include "renderer.h"
#include "../resource/resource_manager.h"
#include "camera.h"
#include "sprite.h"

#include <SDL3/SDL_render.h>
#include <spdlog/spdlog.h>

namespace engine::render {

Renderer::Renderer(SDL_Renderer* renderer, engine::resource::ResourceManager* resource_manager)
    : _renderer(renderer)
    , _resource_manager(resource_manager)
{
    spdlog::debug("Begin to initialize Renderer");

    if (!renderer) {
        throw std::runtime_error("Renderer is nullptr");
    }
    if (!resource_manager) {
        throw std::runtime_error("Resource manager is nullptr");
    }
    setDrawColor(0, 0, 0, 255);
    spdlog::debug("Renderer initialized");
}

void Renderer::drawSprite(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scale, double angle)
{

    auto texture = _resource_manager->getTexture(sprite.getTextureId());
    if (!texture) {
        spdlog::error("Failed to get texture for sprite: {}", sprite.getTextureId());
        return;
    }
    auto src_rect = getSpriteSrcRect(sprite);
    if (!src_rect) {
        spdlog::error("Failed to get source rect for sprite: {}", sprite.getTextureId());
        return;
    }

    // 相机变换
    glm::vec2 render_position = camera.worldToScreen(position);

    // 缩放后的纹理尺寸
    float scaled_x = src_rect->w * scale.x;
    float scaled_y = src_rect->h * scale.y;
    SDL_FRect dst_rect = {
        render_position.x,
        render_position.y,
        scaled_x,
        scaled_y
    };

    if (!isRectInViewport(camera, dst_rect)) {
        // 如果不在视口，那么就不渲染
        return;
    }

    if (!SDL_RenderTextureRotated(_renderer, texture, &src_rect.value(), &dst_rect, angle, nullptr, sprite.isFlipped() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE)) {
        spdlog::error("Failed to render texture for sprite: {}", sprite.getTextureId());
    }
}

void Renderer::drawParallax(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scroll_factor, const glm::bvec2& repeat, const glm::vec2& scale)
{
    auto texture = _resource_manager->getTexture(sprite.getTextureId());
    if (!texture) {
        spdlog::error("Failed to get texture for sprite: {}", sprite.getTextureId());
        return;
    }

    auto src_rect = getSpriteSrcRect(sprite);
    if (!src_rect.has_value()) {
        spdlog::error("Failed to get source rect for sprite: {}", sprite.getTextureId());
        return;
    }

    glm::vec2 position_screen = camera.worldToScreenWithParallax(position, scroll_factor);

    // 计算缩放后的纹理
    float scaled_x = src_rect->w * scale.x;
    float scaled_y = src_rect->h * scale.y;

    glm::vec2 start, stop;
    glm::vec2 viewport_size = camera.getViewportSize();

    if (repeat.x) {
        start.x = glm::mod(position_screen.x, scaled_x) - scaled_x;
        stop.x = viewport_size.x;
    } else {
        start.x = position_screen.x;
        stop.x = glm::min(position_screen.x + scaled_x, viewport_size.x);
    }

    if (repeat.y) {
        start.y = glm::mod(position_screen.y, scaled_y) - scaled_y;
        stop.y = viewport_size.y;
    } else {
        start.y = position_screen.y;
        stop.y = glm::min(position_screen.y + scaled_y, viewport_size.y);
    }

    for (float y = start.y; y < stop.y; y += scaled_y) {
        for (float x = start.x; x < stop.x; x += scaled_x) {
            SDL_FRect dst_rect = {
                x,
                y,
                scaled_x,
                scaled_y
            };

            if (!SDL_RenderTexture(_renderer, texture, nullptr, &dst_rect)) {
                spdlog::warn("Failed to render texture for sprite: {}", sprite.getTextureId());
                return;
            }
        }
    }
}

void Renderer::drawUISprite(const Sprite& sprite, const glm::vec2& position, const std::optional<glm::vec2>& size)
{
    auto texture = _resource_manager->getTexture(sprite.getTextureId());
    if (!texture) {
        spdlog::error("Failed to get texture for sprite: {}", sprite.getTextureId());
        return;
    }

    auto src_rect = getSpriteSrcRect(sprite);
    if (!src_rect.has_value()) {
        spdlog::error("Failed to get source rect for sprite: {}", sprite.getTextureId());
        return;
    }

    SDL_FRect dst_rect = {
        position.x,
        position.y,
        src_rect->w,
        src_rect->h
    };

    if (size.has_value()) {
        dst_rect.w = size.value().x;
        dst_rect.h = size.value().y;
    }

    if (!SDL_RenderTextureRotated(_renderer, texture, &src_rect.value(), &dst_rect, 0, nullptr, sprite.isFlipped() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE)) {
        spdlog::error("Failed to render texture for sprite: {}", sprite.getTextureId());
    }
}

void Renderer::present()
{
    SDL_RenderPresent(_renderer);
}

void Renderer::clearScreen()
{
    if (!SDL_RenderClear(_renderer)) {
        spdlog::error("Failed to clear screen");
    }
}

void Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    if (!SDL_SetRenderDrawColor(_renderer, r, g, b, a)) {
        spdlog::error("Failed to set draw color");
    }
}

void Renderer::setDrawColorFloat(float r, float g, float b, float a)
{
    if (!SDL_SetRenderDrawColorFloat(_renderer, r, g, b, a)) {
        spdlog::error("Failed to set draw color");
    }
}

SDL_Renderer* Renderer::getSDLRenderere() const
{
    return _renderer;
}

std::optional<SDL_FRect> Renderer::getSpriteSrcRect(const Sprite& sprite) const
{
    SDL_Texture* texture = _resource_manager->getTexture(sprite.getTextureId());

    if (!texture) {
        spdlog::error("Failed to get texture for sprite: {}", sprite.getTextureId());
        return std::nullopt;
    }

    std::optional<SDL_FRect> src_rect = sprite.getSourceRect();
    if (src_rect.has_value()) {
        if (src_rect.value().w <= 0 || src_rect.value().h <= 0) {
            spdlog::error("Invalid source rect for sprite: {}", sprite.getTextureId());
            return std::nullopt;
        }
        return src_rect;
    } else {
        SDL_FRect rect = { 0, 0, 0, 0 };
        if (!SDL_GetTextureSize(texture, &rect.w, &rect.h)) {
            spdlog::error("Failed to query texture for sprite: {}", sprite.getTextureId());
            return std::nullopt;
        }
        return rect;
    }
}

bool Renderer::isRectInViewport(const Camera& camera, const SDL_FRect& rect)
{
    glm::vec2 viewport_size = camera.getViewportSize();

    return rect.x + rect.w > 0 && rect.x < viewport_size.x && rect.y + rect.h > 0 && rect.y < viewport_size.y;
}

} // namespace engine::render