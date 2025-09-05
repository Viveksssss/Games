#include "object.h"
#include <algorithm>
#include <iostream>
void Object::handleEvents(SDL_Event& event)
{
    for (auto& child : _children) {
        if (child->isActive()) {
            child->handleEvents(event);
        }
    }
}

void Object::update([[maybe_unused]] float dt)
{
    for (auto& child : _object_to_add) {
        addChild(child);
    }
    _object_to_add.clear();
    for (auto it = _children.begin(); it != _children.end();) {
        auto child = *it;
        if (child->getNeedRemove()) {
            it = _children.erase(it);
            child->clean();
            delete child;
        } else {
            if (child->isActive()) {
                child->update(dt);
            }
            it++;
        }
    }
}

void Object::render()
{
    for (auto& child : _children) {
        if (child->isActive()) {
            child->render();
        }
    }
}

void Object::clean()
{
    SDL_Log("clean!");
    for (auto& child : _children) {
        child->clean();
        delete child;
    }
    _children.clear();
}

void Object::addChild(Object* child)
{
    _children.push_back(child);
}

void Object::safeAddChild(Object* child)
{
    _object_to_add.push_back(child);
}

void Object::removeChild(Object* child)
{
    auto it = std::remove(_children.begin(), _children.end(), child);
    _children.erase(it, _children.end());
}
