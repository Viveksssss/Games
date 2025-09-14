#include "game_scene.h"
#include "../../engine/component/sprite_component.h"
#include "../../engine/component/transform_component.h"
#include "../../engine/core/context.h"
#include "../../engine/object/game_object.h"
#include "../../engine/scene/scene_manager.h"
#include <SDL3/SDL_rect.h>
#include <spdlog/spdlog.h>

namespace game::scene {

GameScene::GameScene(const std::string& scene_name, engine::core::Context& context, engine::scene::SceneManager& sceneManager)
    : Scene(scene_name, context, sceneManager)
{
    spdlog::debug("GameScene::GameScene() called");
}

void GameScene::init()
{
    createTestObject();

    Scene::init();

    spdlog::debug("GameScene::init() called");
}

void GameScene::update(float dt)
{
    Scene::update(dt);
}

void GameScene::render()
{
    Scene::render();
}

void GameScene::handleInput()
{
    Scene::handleInput();
}

void GameScene::clean()
{
    Scene::clean();
}

void GameScene::createTestObject()
{
    spdlog::debug("在 GameScene 中创建 test_object...");
    auto test_object = std::make_unique<engine::object::GameObject>("test_object");

    // 添加组件
    test_object->addComponent<engine::component::TransformComponent>(glm::vec2(100.0f, 100.0f));
    test_object->addComponent<engine::component::SpriteComponent>("assets/textures/Props/big-crate.png", _context.getResourceManager());

    // 将创建好的 GameObject 添加到场景中 （一定要用std::move，否则传递的是左值）
    addGameObject(std::move(test_object));
    spdlog::debug("test_object 创建并添加到 GameScene 中。");
}

}