#pragma once

namespace engine::object {
class GameObject;
}

namespace engine::component {

/**
 * @brief Base class for all components.
 */
class Component {
    friend class engine::object::GameObject;

protected:
    engine::object::GameObject* _owner = nullptr;

public:
    Component() = default;
    virtual ~Component() = default;

    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    Component(Component&&) = delete;
    Component& operator=(Component&&) = delete;

    void setOwner(engine::object::GameObject* owner)
    {
        _owner = owner;
    }

protected:
    virtual void init() { }
    virtual void handleInput() { }
    virtual void update(float) { }
    virtual void render() { }
    virtual void clean() { }
};

}