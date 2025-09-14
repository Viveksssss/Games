#pragma once

#include <memory>
#include <vector>

namespace engine::core {
class Context;
}

namespace engine::scene {
class Scene;

class SceneManager final {

private:
    engine::core::Context& _context;
    std::vector<std::unique_ptr<Scene>> _scene_stack;

    enum class PendingAction { NONE,
        PUSH,
        POP,
        REPLACE };

    /* 接下来的动作和场景，一般一起处理 */
    PendingAction _pending_action = PendingAction::NONE;
    std::unique_ptr<Scene> _pending_scene;

public:
    explicit SceneManager(engine::core::Context& context);

    ~SceneManager();
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

    /* 延时切换场景 */
    void requestPushScene(std::unique_ptr<Scene>&& scene);
    void requestPopScene();
    void requestReplaceScene(std::unique_ptr<Scene>&& scene);

    /* getter */
    Scene* getCurrentScene() const;
    engine::core::Context& getContext() const { return _context; }

    void update(float dt);
    void render();
    void handleInput();
    void close();

private:
    void processPendingAdditions();
    void pushScene(std::unique_ptr<Scene>&& scene);
    void popScene();
    void replaceScene(std::unique_ptr<Scene>&& scene);
};

}