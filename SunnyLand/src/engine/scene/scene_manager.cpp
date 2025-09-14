#include "scene_manager.h"
#include "../core/context.h"
#include "scene.h"
#include <spdlog/spdlog.h>

namespace engine::scene {

SceneManager::SceneManager(engine::core::Context& context)
    : _context(context)
{
    spdlog::debug("SceneManager::SceneManager()");
}

void SceneManager::requestPushScene(std::unique_ptr<Scene>&& scene)
{
    _pending_action = PendingAction::PUSH;
    _pending_scene = std::move(scene);
}

void SceneManager::requestPopScene()
{
    _pending_action = PendingAction::POP;
}

void SceneManager::requestReplaceScene(std::unique_ptr<Scene>&& scene)
{
    _pending_action = PendingAction::REPLACE;
    _pending_scene = std::move(scene);
}

Scene* SceneManager::getCurrentScene() const
{
    if (!_scene_stack.empty()) {
        return _scene_stack.back().get();
    }
    return nullptr;
}

void SceneManager::update(float dt)
{
    Scene* current_scene = getCurrentScene();
    if (current_scene) {
        current_scene->update(dt);
    }
    processPendingAdditions();
}

void SceneManager::render()
{
    for (auto& scene : _scene_stack) {
        if (scene) {
            scene->render();
        }
    }
}

void SceneManager::handleInput()
{
    Scene* current_scene = getCurrentScene();
    if (current_scene) {
        current_scene->handleInput();
    }
}

void SceneManager::close()
{
    spdlog::debug("SceneManager::close()");
    while (!_scene_stack.empty()) {
        if (_scene_stack.back()) {
            spdlog::debug("SceneManager::close() - closing scene {}", _scene_stack.back()->getName());
            _scene_stack.back()->clean();
        }
        _scene_stack.pop_back();
    }
}

void SceneManager::processPendingAdditions()
{
    if (_pending_action == PendingAction::NONE) {
        return;
    }

    switch (_pending_action) {
    case PendingAction::PUSH:
        pushScene(std::move(_pending_scene));
        break;
    case PendingAction::POP:
        popScene();
        break;
    case PendingAction::REPLACE:
        replaceScene(std::move(_pending_scene));
        break;
    default:
        break;
    }
    _pending_action = PendingAction::NONE;
}

void SceneManager::pushScene(std::unique_ptr<Scene>&& scene)
{
    if (!scene) {
        spdlog::warn("SceneManager::pushScene() - nullptr scene");
        return;
    }

    spdlog::debug("SceneManager::pushScene() - pushing scene {}", scene->getName());
    if (!scene->isInitialized()) {
        scene->init();
    }
    _scene_stack.push_back(std::move(scene));
}

void SceneManager::popScene()
{
    if (_scene_stack.empty()) {
        spdlog::warn("SceneManager::popScene() - empty scene stack");
        return;
    }
    spdlog::debug("SceneManager::popScene() - popping scene {}", _scene_stack.back()->getName());
    if (_scene_stack.back()) {
        _scene_stack.back()->clean();
    }
    _scene_stack.pop_back();
}

void SceneManager::replaceScene(std::unique_ptr<Scene>&& scene)
{
    if (!scene) {
        spdlog::warn("SceneManager::replaceScene() - nullptr scene");
        return;
    }
    spdlog::debug("SceneManager::replaceScene() - replacing scene {}", scene->getName());

    while (!_scene_stack.empty()) {
        if (_scene_stack.back()) {
            _scene_stack.back()->clean();
        }
        _scene_stack.pop_back();
    }
    if (!scene->isInitialized()) {
        scene->init();
    }

    _scene_stack.push_back(std::move(scene));
}

SceneManager::~SceneManager()
{
    spdlog::debug("SceneManager::~SceneManager()");
    close();
}

}