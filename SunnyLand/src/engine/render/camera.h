#pragma once

#include "../utils/math.h"
#include <optional>

namespace engine::render {

/**
 * @brief 相机类负责管理相机位置和视口的大小
 */
class Camera final {

private:
    /* 视口大小：渲染窗口大小 */
    glm::vec2 _viewport_size;
    /* 相机位置 */
    glm::vec2 _position;
    /* 限制相机位置 */
    std::optional<engine::utils::Rect> _limit_bounds;

public:
    Camera(const glm::vec2& viewport_size, const glm::vec2& position = { 0.0f, 0.0f }, const std::optional<engine::utils::Rect>& limit_bounds = std::nullopt);

    void update(float dt);
    void move(const glm::vec2& offset);

    glm::vec2 worldToScreen(const glm::vec2& world_pos) const;
    glm::vec2 worldToScreenWithParallax(const glm::vec2& world_pos, const glm::vec2& parallax_factor) const;
    glm::vec2 screenToWorld(const glm::vec2& screen_pos) const;

    void setPosition(const glm::vec2& position);
    void setLimitBounds(const engine::utils::Rect& limit_bounds);

    const glm::vec2& getPosition() const;
    std::optional<engine::utils::Rect> getLimitBounds() const;
    const glm::vec2& getViewportSize() const;

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(Camera&&) = delete;

private:
    /* 实际 进行限制摄相机位置 */
    void clampPosition();
};



}
