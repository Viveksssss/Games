#include "scene.h"
#include "../core/context.h"
#include "../object/game_object.h"
#include "scene_manager.h"
#include <algorithm> // for std::remove_if
#include <spdlog/spdlog.h>

namespace engine::scene {

Scene::Scene(const std::string& name, engine::core::Context& context, engine::scene::SceneManager& scene_manager)
    : _scene_name(name)
    , _context(context)
    , _scene_manager(scene_manager)
{
    spdlog::debug("Scene '{}' created", _scene_name);
}

void Scene::init()
{
    _is_initialized = true;
    spdlog::debug("Scene '{}' initialized", _scene_name);
}

void Scene::update(float dt)
{
    if (!_is_initialized)
        return;
    for (auto it = _game_objects.begin(); it != _game_objects.end();) {
        if (*it && !(*it)->isNeedRemove()) {
            (*it)->update(dt, _context);
            ++it;
        } else {
            if (*it) {
                (*it)->clean();
            }
            it = _game_objects.erase(it);
        }
    }
    processPendingAdditions();
}

void Scene::render()
{
    if (!_is_initialized)
        return;
    for (auto& game_object : _game_objects) {
        if (game_object) {
            game_object->render(_context);
        }
    }
}

void Scene::handleInput()
{
    if (!_is_initialized)
        return;
    for (auto it = _game_objects.begin(); it != _game_objects.end();) {
        if (*it && !(*it)->isNeedRemove()) {
            (*it)->handleInput(_context);
            ++it;
        } else {
            if (*it) {
                (*it)->clean();
            }
            it = _game_objects.erase(it);
        }
    }
}

void Scene::clean()
{
    if (!_is_initialized)
        return;
    for (auto& game_object : _game_objects) {
        if (game_object) {
            game_object->clean();
        }
    }
    _game_objects.clear();
    _is_initialized = false;
    spdlog::debug("Scene '{}' cleaned", _scene_name);
}

void Scene::addGameObject(std::unique_ptr<engine::object::GameObject>&& game_object)
{
    if (game_object) {
        _game_objects.push_back(std::move(game_object));
    } else {
        spdlog::warn("try to add null game to {}", _scene_name);
    }
}

void Scene::safeAddGameObject(std::unique_ptr<engine::object::GameObject>&& game_object)
{
    if (game_object) {
        _pending_additions.push_back(std::move(game_object));
    } else {
        spdlog::warn("try to add null game to {}", _scene_name);
    }
}

void Scene::removeGameObject(engine::object::GameObject* game_object)
{
    if (!game_object) {
        spdlog::warn("try to remove null game from {}", _scene_name);
        return;
    }

    auto it = std::remove_if(_game_objects.begin(), _game_objects.end(), [game_object](const std::unique_ptr<engine::object::GameObject>& ptr) { return ptr.get() == game_object; });

    if (it != _game_objects.end()) {
        (*it)->clean();
        _game_objects.erase(it);
        spdlog::debug("Game object '{}' removed from scene '{}'", game_object->getName(), _scene_name);
    } else {
        spdlog::warn("Failed to remove game object from scene {}. Game object not found", _scene_name);
    }
}

void Scene::safeRemoveGameObject(engine::object::GameObject* game_object)
{
    game_object->setNeedRemove(true);
}

const std::vector<std::unique_ptr<engine::object::GameObject>>& Scene::getGameObjects() const
{
    return _game_objects;
}

engine::object::GameObject* Scene::findGameObjectByName(const std::string& name) const
{
    for (const auto& game_object : _game_objects) {
        if (game_object && game_object->getName() == name) {
            return game_object.get();
        }
    }
    return nullptr;
}

void Scene::processPendingAdditions()
{
    for (auto& game_object : _pending_additions) {
        addGameObject(std::move(game_object));
    }
    _pending_additions.clear();
}

Scene::~Scene() = default;
}
