#include "object.h"
#include "Scene.h"
#include "SceneMain.h"
#include <SDL_events.h>
#include <algorithm>

void Player::keyboardControll(SDL_Window* window, float dt)
{
    if (isDead)
        return;

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    float moveAmount = 1 * speed * dt * 500;
    auto keyboardState = SDL_GetKeyboardState(nullptr);

    if (keyboardState[SDL_SCANCODE_W]) {
        position.y -= moveAmount;
        position.y = std::max(position.y, 0.0f); // 使用浮点数比较
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        position.x -= moveAmount;
        position.x = std::max(position.x, 0.0f);
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        position.y += moveAmount;
        position.y = std::min(position.y, static_cast<float>(windowHeight - height));
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        position.x += moveAmount;
        position.x = std::min(position.x, static_cast<float>(windowWidth - width));
    }
    if (keyboardState[SDL_SCANCODE_SPACE]) {
        if (SDL_GetTicks() - lastShotTime > coolDown) {
            shoot(dt);
            lastShotTime = SDL_GetTicks();
        }
    }
}

void Player::shoot(float dt)
{
    auto p = dynamic_cast<SceneMain*>(scene);
    auto bullet = new Bullet(p->getBulletTemplate());
    bullet->position.x = position.x + width / 2 - bullet->width / 2;
    bullet->position.y = position.y - bullet->height;
    p->getBullets().push_back(bullet);
    Mix_PlayChannel(0, p->getSounds()["player_shoot"], 0);
}

#include <iostream>
void Player::getItem(Item* item)
{
    static_cast<SceneMain*>(scene)->getPlayer().currentScore += 5;
    std::cout << this->speed << std::endl;
    std::cout << this->currentHealth << std::endl;
    if (item->type == ITEM_TYPE::Life) {
        this->currentHealth += 1;
        if (currentHealth > 7) {
            currentHealth = 7;
        }
    } else if (item->type == ITEM_TYPE::Shield) {
        this->currentHealth += 1;
        if (currentHealth > 7) {
            currentHealth = 7;
        }
    } else if (item->type == ITEM_TYPE::Time) {
        this->coolDown -= 2;
        if (this->coolDown >= 20000) {
            this->coolDown = 5;
        }
    }
}

int Player::getScore()
{
    return this->currentScore;
}

Enemy::Enemy(Scene* _scene)
{
    scene = _scene;
}

void Enemy::shoot(float dt)
{
    auto p = dynamic_cast<SceneMain*>(scene);
    auto bullet = new EnemyBullet(p->getEnemyBulletTemplate());
    bullet->position.x = position.x + width / 2 - bullet->width / 2;
    bullet->position.y = position.y + width / 2 - bullet->height / 2;
    bullet->direction = getDirection(p->getPlayer());

    p->getEnemiesBullets().push_back(bullet);
    Mix_PlayChannel(1, p->getSounds()["enemy_shoot"], 0);
}

void Enemy::explode(const Explosion& explosion)
{
    auto p = dynamic_cast<SceneMain*>(scene);
    auto currentTime = SDL_GetTicks();
    auto newExplosion = new Explosion(explosion);
    newExplosion->position.x = position.x + width / 2 - newExplosion->width / 2;
    newExplosion->position.y = position.y + height / 2 - newExplosion->height / 2;
    newExplosion->startTime = currentTime;

    p->getExplosions().push_back(newExplosion);
    Mix_PlayChannel(3, p->getSounds()["enemy_explosion"], 0);

    p->getPlayer().currentScore += 10;
    size_t a = rand() % 9;
    if (a <= 2) {
        if (a == 0)
            dropItem(p->getLifeItemTemplate());
        else if (a == 1)
            dropItem(p->getShieldItemTemplate());
        else if (a == 2)
            dropItem(p->getTimeItemTemplate());
    }

    delete this;
}

void Enemy::dropItem(Item _item)
{
    auto item = new Item(_item);
    item->position.x = position.x + width / 2 - item->width / 2;
    item->position.y = position.y + height / 2 - item->height / 2;
    float angle = (rand() % 360 + 1) * M_PI / 180;
    item->direction.x = std::cos(angle);
    item->direction.y = std::sin(angle);
    static_cast<SceneMain*>(scene)->getItems().push_back(item);
}

SDL_FPoint Enemy::getDirection(Player player)
{
    auto x = player.position.x + player.width / 2 - (position.x + width / 2);
    auto y = player.position.y + player.height / 2 - (position.y + height / 2);
    auto length = std::sqrt(x * x + y * y);
    x /= length;
    y /= length;
    return SDL_FPoint { x, y };
}

void Player::setSpeed(int speed)
{
    this->speed = speed;
}

Player::Player(Scene* _scene)
{
    scene = _scene;
}
