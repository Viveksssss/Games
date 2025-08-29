#include "SceneMain.h"
#include "Game.h"
#include "SceneEnd.h"
#include "SceneTitle.h"
#include "TextRender.h"
#include "object.h"
#include "tool.h"

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_render.h>
#include <random>
#include <string>

void SceneMain::init()
{

    Mix_AllocateChannels(32);

    bgm = Mix_LoadMUS(getPath("assets/music/03_Racing_Through_Asteroids_Loop.ogg").c_str());
    if (bgm == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d:SDL_Error::%s\n", __LINE__, SDL_GetError());
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_PlayMusic(bgm, -1);

    sounds["player_shoot"] = Mix_LoadWAV(getPath("assets/sound/laser_shoot4.wav").c_str());
    sounds["enemy_shoot"] = Mix_LoadWAV(getPath("assets/sound/xs_laser.wav").c_str());
    sounds["player_explosion"] = Mix_LoadWAV(getPath("assets/sound/explosion1.wav").c_str());
    sounds["enemy_explosion"] = Mix_LoadWAV(getPath("assets/sound/explosion3.wav").c_str());
    sounds["get_item"] = Mix_LoadWAV(getPath("assets/sound/eff5.wav").c_str());
    sounds["hit"] = Mix_LoadWAV(getPath("assets/sound/eff11.wav").c_str());

    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

    player.texture = IMG_LoadTexture(game.getRenderer(), getPath("assets/image/SpaceShip.png").c_str());
    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height);
    player.width /= 3;
    player.height /= 3;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height - 20;

    bulletTemplate.texture = IMG_LoadTexture(game.getRenderer(), getPath("assets/image/laser-1.png").c_str());
    SDL_QueryTexture(bulletTemplate.texture, nullptr, nullptr, &bulletTemplate.width, &bulletTemplate.height);
    bulletTemplate.width /= 3;
    bulletTemplate.height /= 3;

    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), getPath("assets/image/insect-2.png").c_str());
    SDL_QueryTexture(enemyTemplate.texture, nullptr, nullptr, &enemyTemplate.width, &enemyTemplate.height);
    enemyTemplate.width /= 3;
    enemyTemplate.height /= 3;

    enemyBulletTemplate.texture = IMG_LoadTexture(game.getRenderer(), getPath("assets/image/bullet-1.png").c_str());
    SDL_QueryTexture(enemyBulletTemplate.texture, nullptr, nullptr, &enemyBulletTemplate.width, &enemyBulletTemplate.height);
    enemyBulletTemplate.width /= 3;
    enemyBulletTemplate.height /= 3;

    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), getPath("assets/effect/explosion.png").c_str());
    SDL_QueryTexture(explosionTemplate.texture, nullptr, nullptr, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totalFrame = explosionTemplate.width / explosionTemplate.height;
    explosionTemplate.height *= 2;
    explosionTemplate.width = explosionTemplate.height;

    lifeItemTemplate.texture = IMG_LoadTexture(game.getRenderer(), getPath("assets/image/bonus_life.png").c_str());
    lifeItemTemplate.type = ITEM_TYPE::Life;
    SDL_QueryTexture(lifeItemTemplate.texture, nullptr, nullptr, &lifeItemTemplate.width, &lifeItemTemplate.height);
    lifeItemTemplate.width /= 3;
    lifeItemTemplate.height /= 3;

    shieldItemTemplate.texture = IMG_LoadTexture(game.getRenderer(), getPath("assets/image/bonus_shield.png").c_str());
    shieldItemTemplate.type = ITEM_TYPE::Shield;
    SDL_QueryTexture(shieldItemTemplate.texture, nullptr, nullptr, &shieldItemTemplate.width, &shieldItemTemplate.height);
    shieldItemTemplate.width /= 3;
    shieldItemTemplate.height /= 3;

    timeItemTemplate.texture = IMG_LoadTexture(game.getRenderer(), getPath("assets/image/bonus_time.png").c_str());
    timeItemTemplate.type = ITEM_TYPE::Time;
    SDL_QueryTexture(timeItemTemplate.texture, nullptr, nullptr, &timeItemTemplate.width, &timeItemTemplate.height);
    timeItemTemplate.width /= 3;
    timeItemTemplate.height /= 3;

    uiHp = IMG_LoadTexture(game.getRenderer(), getPath("assets/image/Health UI Black.png").c_str());
}
void SceneMain::update()
{
    player.keyboardControll(Game::getInstance().getWindow(), Game::getInstance().getDeltaTime());
    updateBullets(Game::getInstance().getDeltaTime());
    spawnEnemy();
    updateEnemy(Game::getInstance().getDeltaTime());
    updateEnemyBullets(Game::getInstance().getDeltaTime());
    updatePlayer(Game::getInstance().getDeltaTime());
    updateExplosion(Game::getInstance().getDeltaTime());
    updateItems(Game::getInstance().getDeltaTime());
    updateLength(Game::getInstance().getDeltaTime());

    if(shouldClose){
        game.changeScene(new SceneEnd(player.getScore()));
    }
}
void SceneMain::render()
{
    // 渲染子弹
    renderBullets();
    // 渲染敌机子弹
    renderEnemyBullets();
    // 渲染玩家
    renderPlayer();
    // 渲染敌人
    renderEnemy();
    // 渲染特效
    renderExplosion();
    // 渲染掉落物
    renderItems();
    // 渲染文字
    renderText();
    // 渲染血条
    renderHp();
}
void SceneMain::clean()
{
    for (auto& [key, value] : sounds) {
        if (value != nullptr) {
            Mix_FreeChunk(value);
            value = nullptr;
        }
    }
    sounds.clear();
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }
    if (uiHp != nullptr) {
        SDL_DestroyTexture(uiHp);
        uiHp = nullptr;
    }
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
    }
    if (bulletTemplate.texture != nullptr) {
        SDL_DestroyTexture(bulletTemplate.texture);
        bulletTemplate.texture = nullptr;
    }
    if (enemyBulletTemplate.texture != nullptr) {
        SDL_DestroyTexture(enemyBulletTemplate.texture);
        enemyBulletTemplate.texture = nullptr;
    }
    if (enemyTemplate.texture != nullptr) {
        SDL_DestroyTexture(enemyTemplate.texture);
        enemyTemplate.texture = nullptr;
    }
    if (explosionTemplate.texture != nullptr) {
        SDL_DestroyTexture(explosionTemplate.texture);
        explosionTemplate.texture = nullptr;
    }
    if (lifeItemTemplate.texture != nullptr) {
        SDL_DestroyTexture(lifeItemTemplate.texture);
        lifeItemTemplate.texture = nullptr;
    }
    if (shieldItemTemplate.texture != nullptr) {
        SDL_DestroyTexture(shieldItemTemplate.texture);
        shieldItemTemplate.texture = nullptr;
    }
    if (timeItemTemplate.texture != nullptr) {
        SDL_DestroyTexture(timeItemTemplate.texture);
        timeItemTemplate.texture = nullptr;
    }
    for (auto& bullet : bullets) {
        if (bullet != nullptr) {
            delete bullet;
            bullet = nullptr;
        }
    }

    bullets.clear();
    for (auto& enemy : enemies) {
        if (enemy != nullptr) {
            delete enemy;
            enemy = nullptr;
        }
    }
    enemies.clear();
    for (auto& enemyBullet : enemyBullets) {
        if (enemyBullet != nullptr) {
            delete enemyBullet;
            enemyBullet = nullptr;
        }
    }
    enemyBullets.clear();
    for (auto& explosion : explosions) {
        if (explosion != nullptr) {
            delete explosion;
            explosion = nullptr;
        }
    }
    explosions.clear();
    for (auto& item : items) {
        if (item != nullptr) {
            delete item;
            item = nullptr;
        }
    }
    items.clear();
}
void SceneMain::handleEvents(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
        game.changeScene(new SceneTitle);
    }
}

template <class T>
bool SceneMain::isIntersection(T* all)
{
    SDL_Rect playerRect = {
        static_cast<int>(player.position.x),
        static_cast<int>(player.position.y),
        player.width,
        player.height
    };
    SDL_Rect allRect = {
        static_cast<int>(all->position.x),
        static_cast<int>(all->position.y),
        all->width,
        all->height
    };
    if (SDL_HasIntersection(&playerRect, &allRect)) {
        return true;
    }
    return false;
}
// // 显式实例化
// template bool SceneMain::isIntersection(Bullet* all);
// template bool SceneMain::isIntersection(Enemy* all);
// template bool SceneMain::isIntersection(EnemyBullet* all);
// template bool SceneMain::isIntersection(Item* all);

void SceneMain::renderBullets()
{
    for (auto bullet : bullets) {
        SDL_Rect dst = {
            static_cast<int>(bullet->position.x),
            static_cast<int>(bullet->position.y),
            bullet->width,
            bullet->height
        };
        SDL_RenderCopy(game.getRenderer(), bullet->texture, nullptr, &dst);
    }
}

void SceneMain::renderEnemy()
{
    for (auto enemy : enemies) {
        SDL_Rect dst = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        SDL_RenderCopy(game.getRenderer(), enemy->texture, nullptr, &dst);
    }
}

void SceneMain::renderEnemyBullets()
{
    for (auto bullet : enemyBullets) {
        SDL_Rect dst = {
            static_cast<int>(bullet->position.x),
            static_cast<int>(bullet->position.y),
            bullet->width,
            bullet->height
        };
        float angle = atan2(bullet->direction.y, bullet->direction.x) * 180 / M_PI - 90;
        // SDL_RenderCopy(game.getRenderer(), bullet->texture, nullptr, &dst);
        SDL_RenderCopyEx(game.getRenderer(), bullet->texture, nullptr, &dst, angle, nullptr, SDL_FLIP_NONE);
    }
}

void SceneMain::renderPlayer()
{
    if (player.isDead == false) {
        SDL_Rect dst = {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height
        };
        SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &dst);
    }
}

void SceneMain::renderExplosion()
{
    for (auto explosion : explosions) {
        SDL_Rect src = {
            static_cast<int>(explosion->currentFrame * explosion->width),
            0,
            explosion->width / 2,
            explosion->height / 2
        };
        SDL_Rect dst = {
            static_cast<int>(explosion->position.x),
            static_cast<int>(explosion->position.y),
            explosion->width,
            explosion->height
        };
        SDL_RenderCopy(game.getRenderer(), explosion->texture, &src, &dst);
    }
}

void SceneMain::renderItems()
{
    for (auto& item : items) {
        SDL_Rect dst = {
            static_cast<int>(item->position.x),
            static_cast<int>(item->position.y),
            item->width,
            item->height
        };
        SDL_RenderCopy(game.getRenderer(), item->texture, nullptr, &dst);
    }
}

void SceneMain::renderText()
{
    std::list<std::string> data = updateText(game.getDeltaTime());
    int x = 5;
    int y = 50;
    auto it = data.begin();
    for (size_t i = 0; i < data.size(); i++) {
        game.textRenderer->renderText(game.getRenderer(), *(it++), x, y + i * 30);
    }
}

void SceneMain::renderHp()
{
    int x = 5;
    int y = 20;
    int offset = 30;
    int size = 32;
    SDL_SetTextureColorMod(uiHp, 100, 100, 100);
    for (int i = 0; i < 7; i++) {
        SDL_Rect dst = {
            x + i * offset,
            y,
            size,
            size
        };
        SDL_RenderCopy(game.getRenderer(), uiHp, nullptr, &dst);
    }
    SDL_SetTextureColorMod(uiHp, 255, 255, 255);
    for (int i = 0; i < player.currentHealth; i++) {
        SDL_Rect dst = {
            x + i * offset,
            y,
            size,
            size
        };
        SDL_RenderCopy(game.getRenderer(), uiHp, nullptr, &dst);
    }
}

void SceneMain::updateBullets(float dt)
{
    for (auto it = bullets.begin(); it != bullets.end();) {
        auto bullet = *it;
        bullet->position.y -= bullet->speed * dt * 1000;
        if (bullet->position.y + 32 < 0) {
            delete bullet;
            it = bullets.erase(it);
        } else {
            bool hit = false;
            for (const auto& enemy : enemies) {
                SDL_Rect enemyRect = {
                    static_cast<int>(enemy->position.x),
                    static_cast<int>(enemy->position.y),
                    enemy->width,
                    enemy->height
                };
                SDL_Rect bulletRect = {
                    static_cast<int>(bullet->position.x),
                    static_cast<int>(bullet->position.y),
                    bullet->width,
                    bullet->height
                };
                if (SDL_HasIntersection(&enemyRect, &bulletRect)) {
                    enemy->currentHealth -= bullet->damage;
                    delete bullet;
                    it = bullets.erase(it);
                    hit = true;
                    Mix_PlayChannel(4, sounds["hit"], 0);
                    break;
                }
            }
            if (!hit)
                ++it;
        }
    }
}

void SceneMain::updateEnemy(float dt)
{
    auto currentTime = SDL_GetTicks();
    for (auto it = enemies.begin(); it != enemies.end();) {
        auto enemy = *it;
        enemy->position.y += enemy->speed * dt * 250;
        if (enemy->position.y > game.getWindowHeight()) {
            delete enemy;
            it = enemies.erase(it);
        } else {
            if (currentTime - enemy->lastShotTime > enemy->coolDown && player.isDead == false) {
                enemy->shoot(dt);
                enemy->lastShotTime = currentTime;
            }
            if (isIntersection(enemy) && player.isDead == false) {
                player.currentHealth -= 1;
                enemy->currentHealth = 0;
            }
            if (enemy->currentHealth <= 0) {
                enemy->explode(explosionTemplate);
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void SceneMain::updateEnemyBullets(float dt)
{
    for (auto it = enemyBullets.begin(); it != enemyBullets.end();) {
        auto enemyBullet = *it;
        enemyBullet->position.y += enemyBullet->speed * dt * 500 * enemyBullet->direction.y;
        enemyBullet->position.x += enemyBullet->speed * dt * 500 * enemyBullet->direction.x;
        if (enemyBullet->position.y > game.getWindowHeight() + 32 || enemyBullet->position.x > game.getWindowWidth() + 32 || enemyBullet->position.x < -32 || enemyBullet->position.y < -32) {
            delete enemyBullet;
            it = enemyBullets.erase(it);
        } else {

            SDL_Rect playerRect = {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height
            };
            SDL_Rect enemyBulletRect = {
                static_cast<int>(enemyBullet->position.x),
                static_cast<int>(enemyBullet->position.y),
                enemyBullet->width,
                enemyBullet->height
            };
            if (SDL_HasIntersection(&playerRect, &enemyBulletRect) && player.isDead == false) {
                player.currentHealth -= enemyBullet->damage;
                delete enemyBullet;
                it = enemyBullets.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void SceneMain::updatePlayer(float dt)
{
    static_cast<void>(dt);
    if (player.isDead == true) {
        timer += dt;
        if (timer >= 2.0f) {
            timer = 0.0f;
            shouldClose = true;
        }
        return;
    }
    if (player.currentHealth <= 0) {
        auto currentTime = SDL_GetTicks();
        player.isDead = true;
        auto explosion = new Explosion(explosionTemplate);
        explosion->position.x = player.position.x + player.width / 2 - explosion->width / 2;
        explosion->position.y = player.position.y + player.height / 2 - explosion->height / 2;
        explosion->startTime = currentTime;
        explosions.push_back(explosion);
        Mix_PlayChannel(2, sounds["player_explosion"], 0);
        return;
    }
}

void SceneMain::updateExplosion([[maybe_unused]] float dt)
{
    auto currentTime = SDL_GetTicks();
    if (explosions.empty())
        return;
    for (auto it = explosions.begin(); it != explosions.end();) {
        Explosion* explosion = nullptr;
        if (*it == nullptr) {
            ++it;
            continue;
        }
        explosion = *it;

        explosion->currentFrame = (currentTime - explosion->startTime) / (1000.0f / explosion->FPS);
        if (explosion->currentFrame >= explosion->totalFrame) {
            delete explosion;
            it = explosions.erase(it);
        } else {
            ++it;
        }
    }
}

void SceneMain::updateItems([[maybe_unused]] float dt)
{
    for (auto it = items.begin(); it != items.end();) {
        auto item = *it;
        // 更新位置
        item->position.y += item->speed * dt * 250 * item->direction.y;
        item->position.x += item->speed * dt * 250 * item->direction.x;
        // 反弹
        if (item->position.x < 0 && item->boundce > 0) {
            item->direction.x = -item->direction.x;
            item->boundce--;
        }
        if (item->position.x > game.getWindowWidth() - item->width && item->boundce > 0) {
            item->direction.x = -item->direction.x;
            item->boundce--;
        }
        if (item->position.y < 0 && item->boundce > 0) {
            item->direction.y = -item->direction.y;
            item->boundce--;
        }
        if (item->position.y > game.getWindowHeight() - item->height && item->boundce > 0) {
            item->direction.y = -item->direction.y;
            item->boundce--;
        }
        // 边界检查
        if (item->boundce <= 0 && (item->position.y > game.getWindowHeight() || item->position.y < -item->height || item->position.x > game.getWindowWidth() - item->width || item->position.x < -item->width)) {
            delete item;
            it = items.erase(it);
        } else {
            if (isIntersection(item) && player.isDead == false) {
                player.getItem(item);
                delete item;
                it = items.erase(it);
                Mix_PlayChannel(5, sounds["get_item"], 0);
            } else {
                ++it;
            }
        }
    }
}

void SceneMain::updateLength([[maybe_unused]] float dt)
{
    if (player.isDead == false)
        player.currentLength += 1;
}

std::list<std::string> SceneMain::updateText([[maybe_unused]] float dt)
{
    std::string hit_speed = "Speed:" + std::to_string(201 - player.coolDown);
    std::string scores = "Scores:" + std::to_string(player.currentScore);
    std::string length = "Length:" + std::to_string(player.currentLength);

    return std::list<std::string> { hit_speed, scores, length };
}

SceneMain::SceneMain()
    : player(nullptr)
    , enemyTemplate(nullptr)
{
    player.scene = this;
    enemyTemplate.scene = this;
}

void SceneMain::spawnEnemy()
{
    if (dis(gen) > 1 / 120.0f) {
        return;
    }
    auto enemy = new Enemy(enemyTemplate);
    enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
    enemy->position.y = -enemy->height;
    enemies.push_back(enemy);
}