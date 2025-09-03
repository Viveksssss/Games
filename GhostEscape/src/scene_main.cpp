#include "scene_main.h"
#include "player.h"
#include "enemy.h"


void SceneMain::init()
{
    this->_world_size = game.getScreenSize() * 3.0f;
    _camera_position = _world_size / 2.0f - game.getScreenSize() / 2.0f;

    player = new Player();
    player->init();
    player->setPosition(_world_size / 2.0f);
    addChild(player);

    for (int i = 0; i < 10; ++i){
        auto enemy = new Enemy();
        enemy->init();
        enemy->setPosition(glm::vec2(rand() % (int)_world_size.x, rand() % (int)_world_size.y));
        enemy->setTarget(player);
        addChild(enemy);
    }
}

void SceneMain::update(float dt)
{
    Scene::update(dt);
}

// 默认构造函数，使用=default关键字显式声明使默认生成的构造函数
void SceneMain::render()
{
    // 先渲染背景，再渲染场景中的对象
    render_background();
    Scene::render();
}

void SceneMain::handleEvents(SDL_Event& event)
{
    Scene::handleEvents(event);
}

void SceneMain::clean()
{
    Scene::clean();
}

void SceneMain::render_background()
{
    auto start = -_camera_position;
    auto end = _world_size - _camera_position;
    game.drawGrid(start, end, 80, SDL_FColor(0.5, 0.5, 0.5, 1.0f));
    game.drawBoundary(start, end, 5.0f, SDL_FColor(0.5, 0.5, 0.5, 1.0f));
}
