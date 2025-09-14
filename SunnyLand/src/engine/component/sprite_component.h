#pragma once

#include "../render/sprite.h"
#include "../utils/alignment.h"
#include "component.h"
#include <SDL3/SDL_rect.h>
#include <glm/glm.hpp>
#include <optional>
#include <string>

namespace engine::core {
class Context;
}

namespace engine::core {
class Context;
}
namespace engine::resource {
class ResourceManager;
}

namespace engine::component {
class TransformComponent;

class SpriteComponent final : public engine::component::Component {
    friend class engine::object::GameObject;

private:
    engine::resource::ResourceManager* _resource_manager = nullptr;
    TransformComponent* _transform = nullptr;

    engine::render::Sprite _sprite;
    engine::utils::Alignment _alignment = engine::utils::Alignment::NONE;
    glm::vec2 _offset = glm::vec2(0.0f, 0.0f);
    glm::vec2 _sprite_size = glm::vec2(0.0f, 0.0f);
    bool _is_hidden = false;

public:
    SpriteComponent(const std::string& texture_id,
        engine::resource::ResourceManager& resource_manager,
        engine::utils::Alignment alignment = engine::utils::Alignment::NONE,
        std::optional<SDL_FRect> source_rect = std::nullopt,
        bool is_flipped = false);

    ~SpriteComponent() override = default;
    SpriteComponent(const SpriteComponent&) = delete;
    SpriteComponent& operator=(const SpriteComponent&) = delete;
    SpriteComponent(SpriteComponent&&) = delete;
    SpriteComponent& operator=(SpriteComponent&&) = delete;

    /**
     * @brief 更新偏移量(根据当前的 alignment_ 和 sprite_size_ 计算 offset_)。
     */
    void updateOffset();

    // getter
    const engine::render::Sprite& getSprite() const { return _sprite; }
    const std::string& getTextureId() const { return _sprite.getTextureId(); }
    bool isHidden() const { return _is_hidden; }
    bool isFlipped() const { return _sprite.isFlipped(); }
    const glm::vec2& getOffset() const { return _offset; }
    const glm::vec2& getSpriteSize() const { return _sprite_size; }
    engine::utils::Alignment getAlignment() const { return _alignment; }

    // setter
    void setSpriteById(const std::string& texture_id, const std::optional<SDL_FRect>& source_rect = std::nullopt);
    void setFlipped(bool is_flipped) { _sprite.setFlipped(is_flipped); }
    void setHidden(bool is_hidden) { _is_hidden = is_hidden; }
    void setAlignment(engine::utils::Alignment alignment);
    void setSourceRect(const std::optional<SDL_FRect>& source_rect);

private:
    void updateSpriteSize();
    void init() override;
    void update(float, engine::core::Context&) override { }
    void render(engine::core::Context&) override;
};

}
