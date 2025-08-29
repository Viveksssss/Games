#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "object.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <list>
#include <map>
#include <random>

class Game;

class SceneMain : public Scene {
public:
    SceneMain();
    ~SceneMain() = default;

    virtual void init() override;
    virtual void update() override;
    virtual void render() override;
    virtual void clean() override;
    virtual void handleEvents(SDL_Event* event) override;

    inline Player& getPlayer() { return this->player; };
    inline std::list<Bullet*>& getBullets() { return bullets; }
    inline std::list<EnemyBullet*>& getEnemiesBullets() { return enemyBullets; }
    inline std::list<Explosion*>& getExplosions() { return explosions; }
    inline std::list<Item*>& getItems() { return items; }
    inline Bullet getBulletTemplate() { return bulletTemplate; }
    inline EnemyBullet getEnemyBulletTemplate() { return enemyBulletTemplate; }
    inline Item getLifeItemTemplate() { return lifeItemTemplate; }
    inline Item getShieldItemTemplate() { return shieldItemTemplate; }
    inline Item getTimeItemTemplate() { return timeItemTemplate; }
    inline std::map<std::string, Mix_Chunk*>& getSounds() { return sounds; }

private:
    // 延时切换计时器
    float timer = 0.0f;

    // 游戏对象
    Player player;
    std::list<Bullet*> bullets;
    std::list<Enemy*> enemies;
    std::list<EnemyBullet*> enemyBullets;
    std::list<Explosion*> explosions;
    std::list<Item*> items;

    // 模板
    Bullet bulletTemplate;
    Enemy enemyTemplate;
    EnemyBullet enemyBulletTemplate;
    Explosion explosionTemplate;
    Item lifeItemTemplate;
    Item shieldItemTemplate;
    Item timeItemTemplate;

    // 随机数生成器
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    bool shouldClose = false;

    // 资源
    // 背景音乐
    Mix_Music* bgm;
    // 音效
    std::map<std::string, Mix_Chunk*> sounds;
    // HPui
    SDL_Texture* uiHp;

    // 检测玩家和敌机
    template <class T>
    bool isIntersection(T* all);

    // 渲染
    void renderBullets();
    void renderEnemy();
    void renderEnemyBullets();
    void renderPlayer();
    void renderExplosion();
    void renderItems();
    void renderText();
    void renderHp();

    // 更新
    void spawnEnemy();
    void updateBullets([[maybe_unused]] float dt);
    void updateEnemy([[maybe_unused]] float dt);
    void updateEnemyBullets([[maybe_unused]] float dt);
    void updatePlayer([[maybe_unused]] float dt);
    void updateExplosion([[maybe_unused]] float dt);
    void updateItems([[maybe_unused]] float dt);
    void updateLength([[maybe_unused]] float dt);
    std::list<std::string> updateText([[maybe_unused]] float dt);
};

#endif