#include "game_object.h"

namespace engine::object {

GameObject::GameObject(const std::string& name, const std::string& tag)
    : _name(name)
    , _tag(tag)
{
    spdlog::debug("GameObject::GameObject() called, name: {}, tag: {}", _name, _tag);
}

void GameObject::update(float dt)
{
    for (auto& pair : _components) {
        pair.second->update(dt);
    }
}

void GameObject::render()
{
    for (auto& pair : _components) {
        pair.second->render();
    }
}

void GameObject::clean()
{
    for (auto& pair : _components) {
        pair.second->clean();
    }
    _components.clear();
}

void GameObject::handleInput()
{
    for (auto& pair : _components) {
        pair.second->handleInput();
    }
}

} // namespace engine::object
