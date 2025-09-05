#include "scene_main.h"
#include "enemy.h"
#include "player.h"
#include "screen/ui_mouse.h"
#include "spawner.h"
#include "world/spell.h"

void SceneMain::init()
{

    this->_world_size = game.getScreenSize() * 3.0f;
    _camera_position = _world_size / 2.0f - game.getScreenSize() / 2.0f;

    player = new Player();
    player->init();
    player->setPosition(_world_size / 2.0f);
    addChild(player);

    spawner = new Spawner();
    spawner->init();
    spawner->setPlayer(player);
    addChild(spawner);

    ui_mouse = UIMouse::create(this, "assets/UI/29.png", "assets/UI/30.png", 1.0f, Anchor::CENTER);
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
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            auto mouse_pos = game.getMousePosition();
            auto world_pos = screenToWorld(mouse_pos);
            Spell::create(this, "assets/effect/Thunderstrike w blur.png", world_pos, 120.0f,3.0f, Anchor::CENTER);
        }
    }
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
