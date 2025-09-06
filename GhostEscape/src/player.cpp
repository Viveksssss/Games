#include "player.h"
#include "affiliate/collider.h"
#include "affiliate/sprite_anim.h"
#include "core/scene.h"
#include "raw/stats.h"
#include "weapon_thunder.h"
#include "world/effect.h"

void Player::init()
{
    Actor::init();
    max_speed = 500.0f;

    // 设置静止和移动的动画
    this->_sprite_idle = SpriteAnim::create(this, "assets/sprite/ghost-idle.png", 2.0f);
    this->_sprite_move = SpriteAnim::create(this, "assets/sprite/ghost-move.png", 2.0f);

    this->_sprite_move->setActive(false);
    this->_sprite_idle->setActive(true);

    // 属性
    stats = Stats::create(this, 100.0f, 100.0f, 40.0f, 10.0f);
    // 碰撞盒
    _collider = Collider::create(this, _sprite_idle->getSize() / 1.5f);
    // 死亡效果
    _effect_die = Effect::create(nullptr, "assets/effect/1764.png", glm::vec2(0), 2.0f, nullptr);
    // 武器
    _weapon_thunder = WeaponThunder::create(this, 2.0f, 40.0f);
}

void Player::update([[maybe_unused]] float dt)
{
    Actor::update(dt);
    // 惯性速度
    velocity *= 0.9f;
    // 键盘控制
    keybordControl();
    // 检查运动状态
    checkStates();
    // 移动
    move(dt);
    // 同步相机
    syncCamera();
    // 检查是否死亡
    checkDead();
}

void Player::render()
{
    Actor::render();
}

void Player::clean()
{
    Actor::clean();
}

void Player::handleEvents(SDL_Event& event)
{
    Actor::handleEvents(event);
}

void Player::keybordControl()
{
    auto currentKeyboardState = SDL_GetKeyboardState(nullptr);
    if (currentKeyboardState[SDL_SCANCODE_W]) {
        velocity.y = -max_speed;
    }
    if (currentKeyboardState[SDL_SCANCODE_S]) {
        velocity.y = max_speed;
    }
    if (currentKeyboardState[SDL_SCANCODE_A]) {
        velocity.x = -max_speed;
    }
    if (currentKeyboardState[SDL_SCANCODE_D]) {
        velocity.x = max_speed;
    }
}

void Player::syncCamera()
{
    game.getCurrentScene()->setCameraPosition(getPosition() - game.getScreenSize() / 2.0f);
}

void Player::checkStates()
{
    if (velocity.x < 0) {
        _sprite_move->setFilp(true);
        _sprite_idle->setFilp(true);
    } else {
        _sprite_move->setFilp(false);
        _sprite_idle->setFilp(false);
    }

    bool new_is_moving = (glm::length(velocity) > 0.1f);
    if (new_is_moving != is_moving) {
        is_moving = new_is_moving;
        changeStates(is_moving);
    }
}

void Player::changeStates(bool is_moving)
{
    if (is_moving) {
        _sprite_move->setActive(true);
        _sprite_idle->setActive(false);

        _sprite_move->setCurrentFrame(_sprite_move->getCurrentFrame());
        _sprite_move->setFrameTimer(_sprite_idle->getFrameTimer());
    } else {
        _sprite_move->setActive(false);
        _sprite_idle->setActive(true);

        _sprite_idle->setCurrentFrame(_sprite_idle->getCurrentFrame());
        _sprite_idle->setFrameTimer(_sprite_idle->getFrameTimer());
    }
}

void Player::checkDead()
{
    if (!isAlive()) {
        game.getCurrentScene()->safeAddChild(_effect_die);
        _effect_die->setPosition(getPosition());
        setActive(false);
    }
}
