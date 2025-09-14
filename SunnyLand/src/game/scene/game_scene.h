#pragma once

#include "../../engine/scene/scene.h"

namespace engine::object {
class GameObject;
}

namespace game::scene {

class GameScene final : public engine::scene::Scene {

public:
    GameScene(const std::string& scene_name, engine::core::Context& context, engine::scene::SceneManager& sceneManager);

    void init() override;
    void update(float dt) override;
    void render() override;
    void handleInput() override;
    void clean() override;

private:
    void createTestObject();
};

}