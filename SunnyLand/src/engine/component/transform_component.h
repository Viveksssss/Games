#pragma once

#include "./component.h"
#include <glm/vec2.hpp>

namespace engine::component {

class TransformComponent final : public Component {

    friend class engine::object::GameObject;

public:
    glm::vec2 _position = { 0.0f, 0.0f };
    glm::vec2 _scale = { 1.0f, 1.0f };
    float _rotation = 0.0f;

    TransformComponent(glm::vec2 position = glm::vec2(0.0f, 0.0f), glm::vec2 scale = glm::vec2(1.0f, 1.0f), float rotation = 0.0f)
        : _position(position)
        , _scale(scale)
        , _rotation(rotation)
    {
    }

    TransformComponent(const TransformComponent& other) = delete;
    TransformComponent& operator=(const TransformComponent& other) = delete;
    TransformComponent(TransformComponent&& other) = delete;
    TransformComponent& operator=(TransformComponent&& other) = delete;

    const glm::vec2& getPosition() const { return _position; }
    float getRotation() const { return _rotation; }
    const glm::vec2 getScale() const { return _scale; }
    void setPosition(const glm::vec2& position) { _position = position; }
    void setRotation(float rotation) { _rotation = rotation; }
    void setScale(const glm::vec2& scale);
    void translate(const glm::vec2& offset) { _position += offset; }

private:
    void update(float, engine::core::Context&) override { }
};

}