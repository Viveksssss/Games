#pragma once

#include "../component/component.h"
#include <memory>
#include <spdlog/spdlog.h>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace engine::object {

/**
 * @brief This class represents a game object.
 */
class GameObject {

private:
    std::string _name;
    std::string _tag;
    std::unordered_map<std::type_index, std::unique_ptr<engine::component::Component>> _components;
    /* 延迟删除的标记 */
    bool _need_remove = false;

public:
    GameObject(const std::string& name = "", const std::string& tag = "");

    GameObject(const GameObject& other) = delete;
    GameObject& operator=(const GameObject& other) = delete;
    GameObject(GameObject&& other) = delete;
    GameObject& operator=(GameObject&& other) = delete;

    /* setter and getter */
    inline void setName(const std::string& name) { _name = name; }
    inline const std::string& getName() const { return _name; }
    inline void setTag(const std::string& tag) { _tag = tag; }
    inline const std::string& getTag() const { return _tag; }
    inline void setNeedRemove(bool need_remove) { _need_remove = need_remove; }
    inline bool getNeedRemove() const { return _need_remove; }

    /**
     * @brief 添加组件
     * @tparam T 组件类型
     * @tparam ...Args 组件构造函数参数类型
     * @param ...args 组件构造函数参数
     * @return 组件指针
     */
    template <typename T, typename... Args>
    T* addComponent(Args&&... args)
    {
        static_assert(std::is_base_of_v<engine::component::Component, T>, " T must be a subclass of Component");
        auto type_index = std::type_index(typeid(T));
        if (hasComponent<T>()) {
            return getComponent<T>();
        }

        auto new_component = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = new_component.get();
        new_component->setOwner(this);
        _components.insert(std::make_pair(type_index, std::move(new_component)));
        ptr->init();
        spdlog::debug("GameObject::addComponent<{}> success", typeid(T).name());
        return ptr;
    }

    template <typename T>
    bool hasComponent() const
    {
        static_assert(std::is_base_of_v<engine::component::Component, T>, " T must be a subclass of Component");
        // return _components.find(std::type_index(typeid(T))) != _components.end();
        return _components.contains(std::type_index(typeid(T)));
    }

    template <typename T>
    T* getComponent() const
    {
        static_assert(std::is_base_of_v<engine::component::Component, T>, " T must be a subclass of Component");
        auto type_index = std::type_index(typeid(T));
        if (auto it = _components.find(type_index); it != _components.end()) {
            return dynamic_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template <typename T>
    void removeComponent()
    {
        static_assert(std::is_base_of_v<engine::component::Component, T>, " T must be a subclass of Component");
        auto type_index = std::type_index(typeid(T));
        if (auto it = _components.find(type_index); it != _components.end()) {
            _components.erase(it);
            spdlog::debug("GameObject::removeComponent<{}> success", typeid(T).name());
        }
    }

    void update(float, engine::core::Context&);
    void render(engine::core::Context&);
    void clean();
    void handleInput(engine::core::Context&);

private:
};

}