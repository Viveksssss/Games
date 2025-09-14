#include "transform_component.h"
#include "../object/game_object.h"
#include "sprite_component.h"

namespace engine::component {
void TransformComponent::setScale(const glm::vec2& scale)
{
    _scale = scale;
    if (_owner) {
        auto _sprite = _owner->getComponent<SpriteComponent>();
        if (_sprite) {
            _sprite->updateOffset();
        }
    }
}
}