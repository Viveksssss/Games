#include "scene_main.h"
#include "enemy.h"
#include "player.h"
#include "raw/timer.h"
#include "scene_title.h"
#include "screen/hud_button.h"
#include "screen/hud_stats.h"
#include "screen/ui_mouse.h"
#include "spawner.h"

void SceneMain::init()
{

    this->_world_size = game.getScreenSize() * 3.0f;
    _camera_position = _world_size / 2.0f - game.getScreenSize() / 2.0f;

    // 玩家
    player = new Player();
    player->init();
    player->setPosition(_world_size / 2.0f);
    addChild(player);

    // 敌方生成器
    spawner = new Spawner();
    spawner->init();
    spawner->setPlayer(player);
    addChild(spawner);

    // 计时器
    end_timer = Timer::create(this);

    // HUDButtons
    btn_pause = HUDButton::create(this, game.getScreenSize() - glm::vec2(500.0f, 50.f), "assets/UI/A_Pause1.png", "assets/UI/A_Pause2.png", "assets/UI/A_Pause3.png", 2.0f);
    btn_restart = HUDButton::create(this, game.getScreenSize() - glm::vec2(300.0f, 50.f), "assets/UI/A_Restart1.png", "assets/UI/A_Restart2.png", "assets/UI/A_Restart3.png", 2.0f);
    btn_back = HUDButton::create(this, game.getScreenSize() - glm::vec2(100.0f, 50.f), "assets/UI/A_Back1.png", "assets/UI/A_Back2.png", "assets/UI/A_Back3.png", 2.0f);

    // HUDStats
    hud_stats = HUDStats::create(this, player, { 30.0f, 30.0f });

    // HUDText
    hud_text = HUDText::create(this, "Score:0", glm::vec2(game.getScreenSize().x - 200.0f, 50.0f), glm::vec2(200, 50));

    // 鼠标ui
    ui_mouse = UIMouse::create(this, "assets/UI/29.png", "assets/UI/30.png", 1.0f, Anchor::CENTER);

    // bgm
    game.playMusic("assets/bgm/OhMyGhost.ogg");
}

void SceneMain::update(float dt)
{
    Scene::update(dt);
    update_score();
    checkBtnPause();
    checkBtnRestart();
    checkBtnBack();
    if (player && !player->isActive())
        end_timer->start();
    checkEndTimer();
}

// 默认构造函数，使用=default关键字显式声明使默认生成的构造函数
void SceneMain::render()
{
    // 先渲染背景，再渲染场景中的对象
    render_background();
    Scene::render();
}

bool SceneMain::handleEvents(SDL_Event& event)
{
    if (!Scene::handleEvents(event)) {
        return false;
    }
    return true;
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

void SceneMain::update_score()
{
    hud_text->adaptBgSize();
    hud_text->setText("Score:" + std::to_string(game.getScore()));
}

void SceneMain::checkBtnPause()
{
    if (btn_pause->isTrigger()) {
        if (!_is_pause) {
            pause();
        } else {
            resume();
        }
    }
}

void SceneMain::checkBtnRestart()
{
    if (btn_restart->isTrigger()) {
        auto scene = new SceneMain();
        game.safeChangeScene(scene);
    }
}

void SceneMain::checkBtnBack()
{
    if (btn_back->isTrigger()) {
        auto scene = new SceneTitle();
        game.safeChangeScene(scene);
    }
}

void SceneMain::checkEndTimer()
{
    if (!end_timer->timeOut())
        return;
    pause();
    btn_restart->setRenderPosition(game.getScreenSize() / 2.0f - glm::vec2(300.0f, 0));
    btn_restart->setScale(3);
    btn_back->setRenderPosition(game.getScreenSize() / 2.0f + glm::vec2(300.0f, 0));
    btn_back->setScale(3);
    btn_pause->setActive(false);

    end_timer->stop();
}
