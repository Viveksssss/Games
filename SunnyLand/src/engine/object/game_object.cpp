#include "game_object.h"

namespace engine::object {

GameObject::GameObject(const std::string& name, const std::string& tag)
    : _name(name)
    , _tag(tag)
{
    spdlog::debug("GameObject::GameObject() called, name: {}, tag: {}", _name, _tag);
}

void GameObject::update(float dt, engine::core::Context& context)
{
    for (auto& pair : _components) {
        pair.second->update(dt, context);
    }
}

void GameObject::render(engine::core::Context& context)
{
    for (auto& pair : _components) {
        pair.second->render(context);
    }
}

void GameObject::clean()
{
    for (auto& pair : _components) {
        pair.second->clean();
    }
    _components.clear();
}

void GameObject::handleInput(engine::core::Context& context)
{
    for (auto& pair : _components) {
        pair.second->handleInput(context);
    }
}

} // namespace engine::object
