#include "camera.h"
#include <spdlog/spdlog.h>

namespace engine::render {

Camera::Camera(const glm::vec2& viewport_size, const glm::vec2& position, const std::optional<engine::utils::Rect>& limit_bounds)
    : _viewport_size(viewport_size)
    , _position(position)
    , _limit_bounds(limit_bounds)
{
    spdlog::debug("Camera created x: {},y:{}", position.x, position.y);
}

void Camera::update(float dt)
{
    // TODO: 在此处插入更新逻辑
}

void Camera::move(const glm::vec2& offset)
{
    _position += offset;
    clampPosition();
}

glm::vec2 Camera::worldToScreen(const glm::vec2& world_pos) const
{
    return world_pos - _position;
}

glm::vec2 Camera::worldToScreenWithParallax(const glm::vec2& world_pos, const glm::vec2& parallax_factor) const
{
    return world_pos - _position * parallax_factor;
}

glm::vec2 Camera::screenToWorld(const glm::vec2& screen_pos) const
{
    return _position + screen_pos;
}

void Camera::setPosition(const glm::vec2& position)
{
    _position = position;
    clampPosition();
}

void Camera::setLimitBounds(const engine::utils::Rect& limit_bounds)
{
    _limit_bounds = limit_bounds;
    clampPosition();
}

const glm::vec2& Camera::getPosition() const
{
    return _position;
}

std::optional<engine::utils::Rect> Camera::getLimitBounds() const
{
    return _limit_bounds;
}

const glm::vec2& Camera::getViewportSize() const
{
    return _viewport_size;
}

void Camera::clampPosition()
{
    if (_limit_bounds.has_value()) {
        if (_limit_bounds->size.x > 0 && _limit_bounds->size.y > 0) {
            glm::vec2 min_cam_pos = _limit_bounds->position;
            glm::vec2 max_cam_pos = _limit_bounds->position + _limit_bounds->size - _viewport_size;

            max_cam_pos.x = std::max(max_cam_pos.x, min_cam_pos.x);
            max_cam_pos.y = std::max(max_cam_pos.y, min_cam_pos.y);

            _position = glm::clamp(_position, min_cam_pos, max_cam_pos);
        }
    }
}
} // namespace engine::render
