#include "scene.h"
#include <algorithm>
void Scene::setCameraPosition(const glm::vec2& position)
{
    _camera_position = position;
    _camera_position = glm::clamp(position, glm::vec2(-80), _world_size - game.getScreenSize() + glm::vec2(80.0f));
}

void Scene::addChild(Object* child)
{
    switch (child->getType()) {
    case ObjectType::NONE:
        this->_children.push_back(static_cast<Object*>(child));
        break;
    case ObjectType::OBJECT_SCREEN:
        this->_children_screen.push_back(static_cast<ObjectScreen*>(child));
        break;
    case ObjectType::OBJECT_WORLD:
    case ObjectType::ENEMY:
        this->_children_world.push_back(static_cast<ObjectWorld*>(child));
        break;
    default:
        _children.push_back(static_cast<Object*>(child));
        break;
    }
}

void Scene::removeChild(Object* child)
{
    switch (child->getType()) {
    case ObjectType::NONE:
        this->_children.erase(std::remove(this->_children.begin(), this->_children.end(), child), this->_children.end());
        break;
    case ObjectType::OBJECT_SCREEN:
        this->_children_screen.erase(std::remove(this->_children_screen.begin(), this->_children_screen.end(), child), this->_children_screen.end());
        break;
    case ObjectType::OBJECT_WORLD:
        this->_children_world.erase(std::remove(this->_children_world.begin(), this->_children_world.end(), child), this->_children_world.end());
        break;
    default:
        this->_children_world.erase(std::remove(this->_children_world.begin(), this->_children_world.end(), child), this->_children_world.end());
        break;
    }
}

void Scene::update([[maybe_unused]] float dt)
{
    if (!_is_pause) {
        Object::update(dt);
        for (auto it = _children_world.begin(); it != _children_world.end();) {
            auto child = *it;
            if (child->getNeedRemove()) {
                it = _children_world.erase(it);
                child->clean();
                delete child;
                child = nullptr;
            } else {
                if (child->isActive()) {
                    child->update(dt);
                }
                it++;
            }
        }
    }
    for (auto it = _children_screen.begin(); it != _children_screen.end();) {
        auto child = *it;
        if (child->getNeedRemove()) {
            it = _children_screen.erase(it);
            child->clean();
            delete child;
            child = nullptr;
        } else {
            if (child->isActive()) {
                child->update(dt);
            }
            it++;
        }
    }
}

void Scene::render()
{
    Object::render();
    for (auto& child : _children_world) {
        if (child->isActive()) {
            child->render();
        }
    }
    for (auto& child : _children_screen) {
        if (child->isActive()) {
            child->render();
        }
    }
}

bool Scene::handleEvents(SDL_Event& event)
{
    for (auto& child : _children_screen) {
        if (child->isActive()) {
            if (child->handleEvents(event))
                return true;
        }
    }
    if (_is_pause)
        return false;
    Object::handleEvents(event);
    for (auto& child : _children_world) {
        if (child->isActive()) {
            if (child->handleEvents(event))
                return true;
        }
    }

    return false;
}

void Scene::clean()
{
    Object::clean();
    for (auto& child : _children) {
        child->clean();
        delete child;
        child = nullptr;
    }
    _children.clear();
    for (auto& child : _children_world) {
        child->clean();
        delete child;
        child = nullptr;
    }
    _children_world.clear();
    for (auto& child : _children_screen) {
        child->clean();
        delete child;
        child = nullptr;
    }
    _children_screen.clear();
    for (auto& child : _object_to_add) {
        child->clean();
        delete child;
        child = nullptr;
    }
    _object_to_add.clear();
}

void Scene::resume()
{
    _is_pause = false;
    game.resumeMusic();
    game.resumeSound();
}

void Scene::pause()
{
    _is_pause = true;
    game.pauseMusic();
    game.pauseSound();
}
