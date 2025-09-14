#include "sprite_component.h"
#include "../core/context.h"
#include "../object/game_object.h"
#include "../render/renderer.h"
#include "../resource/resource_manager.h"
#include "transform_component.h"
#include <spdlog/spdlog.h>

namespace engine::component {

SpriteComponent::SpriteComponent(
    const std::string& texture_id,
    engine::resource::ResourceManager& resource_manager,
    engine::utils::Alignment alignment,
    std::optional<SDL_FRect> source_rect,
    bool is_flipped)
    : _resource_manager(&resource_manager)
    , _sprite(texture_id, source_rect, is_flipped)
    , _alignment(alignment)
{
    if (!_resource_manager) {
        spdlog::critical("ResourceManager is nullptr");
    }
    updateOffset();
}

void SpriteComponent::init()
{
    if (!_owner) {
        spdlog::error("Owner is nullptr");
        return;
    }
    _transform = _owner->getComponent<TransformComponent>();
    if (!_transform) {
        spdlog::warn("TransformComponent not found in owner");
        return;
    }
    updateSpriteSize();
    updateOffset();
}

void SpriteComponent::updateOffset()
{
    if (_sprite_size.x == 0 || _sprite_size.y == 0) {
        _offset = glm::vec2(0.0f, 0.0f);
        return;
    }

    auto scale = _transform->getScale();
    switch (_alignment) {
    case engine::utils::Alignment::TOP_LEFT:
        _offset = glm::vec2(0.0f, 0.0f) * scale;
        break;
    case engine::utils::Alignment::TOP_CENTER:
        _offset = glm::vec2(-_sprite_size.x / 2.0f, 0.0f);
        break;
    case engine::utils::Alignment::TOP_RIGHT:
        _offset = glm::vec2(-_sprite_size.x, 0.0f);
        break;
    case engine::utils::Alignment::MIDDLE_LEFT:
        _offset = glm::vec2(0.0f, -_sprite_size.y / 2.0f);
        break;
    case engine::utils::Alignment::MIDDLE_CENTER:
        _offset = glm::vec2(-_sprite_size.x / 2.0f, -_sprite_size.y / 2.0f);
        break;
    case engine::utils::Alignment::MIDDLE_RIGHT:
        _offset = glm::vec2(-_sprite_size.x, -_sprite_size.y / 2.0f);
        break;
    case engine::utils::Alignment::BOTTOM_LEFT:
        _offset = glm::vec2(0.0f, -_sprite_size.y);
        break;
    case engine::utils::Alignment::BOTTOM_CENTER:
        _offset = glm::vec2(-_sprite_size.x / 2.0f, -_sprite_size.y);
        break;
    case engine::utils::Alignment::BOTTOM_RIGHT:
        _offset = glm::vec2(-_sprite_size.x, -_sprite_size.y);
        break;
    case engine::utils::Alignment::NONE:
    default:
        break;
    }
}

void SpriteComponent::setSpriteById(const std::string& texture_id, const std::optional<SDL_FRect>& source_rect)
{
    _sprite.setTextureId(texture_id);
    _sprite.setSourceRect(source_rect);

    updateSpriteSize();
    updateOffset();
}

void SpriteComponent::setAlignment(engine::utils::Alignment alignment)
{
    _alignment = alignment;
    updateOffset();
}

void SpriteComponent::setSourceRect(
    const std::optional<SDL_FRect>& source_rect)
{
    _sprite.setSourceRect(source_rect);
    updateSpriteSize();
    updateOffset();
}

void SpriteComponent::updateSpriteSize()
{
    if (!_resource_manager) {
        spdlog::error("ResourceManager is nullptr");
        return;
    }
    if (_sprite.getSourceRect().has_value()) {
        const auto& src_rect = _sprite.getSourceRect().value();
        _sprite_size = glm::vec2(src_rect.w, src_rect.h);
    } else {
        _sprite_size = _resource_manager->getTextureSize(_sprite.getTextureId());
    }
}

void SpriteComponent::render(engine::core::Context& context)
{
    engine::component::Component::render(context);
    if (_is_hidden || !_transform || !_resource_manager) {
        return;
    }

    const glm::vec2& pos = _transform->getPosition() + _offset;
    const glm::vec2& scale = _transform->getScale();
    float rotation = _transform->getRotation();

    context.getRenderer().drawSprite(context.getCamera(), _sprite, pos, scale, rotation);
}

} // namespace engine::component