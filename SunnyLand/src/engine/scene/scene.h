#pragma once

#include <memory>
#include <string>
#include <vector>

namespace engine::core {
class Context;
}

namespace engine::render {
class Renderer;
class Camera;
}

namespace engine::input {
class InputManager;
}

namespace engine::object {
class GameObject;
}

namespace engine::scene {
class SceneManager;

class Scene {

protected:
    std::string _scene_name;
    engine::core::Context& _context;
    engine::scene::SceneManager& _scene_manager;
    bool _is_initialized = false;
    /* 场景中的对象 */
    std::vector<std::unique_ptr<engine::object::GameObject>> _game_objects;
    /* 延时添加对象 */
    std::vector<std::unique_ptr<engine::object::GameObject>> _pending_additions;

public:
    Scene(const std::string& name, engine::core::Context& context, engine::scene::SceneManager& scene_manager);

    virtual ~Scene();

    Scene(const Scene& other) = delete;
    Scene& operator=(const Scene& other) = delete;
    Scene(Scene&& other) = delete;
    Scene& operator=(Scene&& other) = delete;

    virtual void init();
    virtual void update(float dt);
    virtual void render();
    virtual void handleInput();
    virtual void clean();

    /* 直接向场景中添加一个游戏对象。（初始化时可用，游戏进行中不安全） （&&表示右值引用，与std::move搭配使用，避免拷贝） */
    virtual void addGameObject(std::unique_ptr<engine::object::GameObject>&& game_object);

    virtual void safeAddGameObject(std::unique_ptr<engine::object::GameObject>&& game_object);

    virtual void removeGameObject(engine::object::GameObject* game_object);

    virtual void safeRemoveGameObject(engine::object::GameObject* game_object);

    const std::vector<std::unique_ptr<engine::object::GameObject>>& getGameObjects() const;

    engine::object::GameObject* findGameObjectByName(const std::string& name) const;

    void setName(const std::string& name) { _scene_name = name; }
    const std::string& getName() const { return _scene_name; }
    void setInitialized(bool is_initialized) { _is_initialized = is_initialized; }
    bool isInitialized() const { return _is_initialized; }

    engine::core::Context& getContext() { return _context; }
    engine::scene::SceneManager& getSceneManager() { return _scene_manager; }
    std::vector<std::unique_ptr<engine::object::GameObject>>& getPendingAdditions() { return _pending_additions; }

private:
    /* 每轮最后调用处理带添加游戏对象 */
    void processPendingAdditions();
};

}