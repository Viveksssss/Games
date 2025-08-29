#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
#include <SDL_render.h>
#include <SDL_video.h>

class Scene;
class Explosion;
class Item;
class Player {
public:
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = { 0, 0 };
    int width;
    int height;
    bool isDead = false;

    int speed = 1;
    int currentHealth = 3;
    int currentLength = 0;
    int currentScore = 0;

    Scene* scene;

    Uint32 coolDown = 200;
    Uint32 lastShotTime;

    Player(Scene* scene);

    void setSpeed(int speed);
    void keyboardControll(SDL_Window* window, float dt);
    void shoot(float dt);
    void getItem(Item* item);
    int getScore();
};

class Bullet {
public:
    int width;
    int height;
    int speed = 1;
    SDL_FPoint position = { 0, 0 };
    SDL_Texture* texture = nullptr;
    int damage = 1;
};

class Enemy {
public:
    Enemy(Scene*);
    int width;
    int height;
    int speed = 1;
    int currentHealth = 2;

    Uint32 coolDown = 2000;
    Uint32 lastShotTime;

    Scene* scene;

    SDL_FPoint position = { 0, 0 };
    SDL_Texture* texture = nullptr;

    void shoot(float dt);
    void explode(const Explosion& explosion);
    void dropItem(Item item);
    SDL_FPoint getDirection(Player player);
};

class EnemyBullet {
public:
    int width;
    int height;
    int speed = 1;
    SDL_FPoint position = { 0, 0 };
    SDL_FPoint direction = { 0, 0 };
    SDL_Texture* texture = nullptr;
    int damage = 1;
};

class Explosion {
public:
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = { 0, 0 };
    int width;
    int height;
    int currentFrame = 0;
    int totalFrame = 0;
    Uint32 startTime = 0;
    Uint32 FPS = 10;
};

enum class ITEM_TYPE {
    Life,
    Shield,
    Time
};
class Item {
public:
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = { 0, 0 };
    SDL_FPoint direction = { 0, 0 };
    int speed = 1;
    int boundce = 3;
    int width;
    int height;
    ITEM_TYPE type = ITEM_TYPE::Life;
};

class Background {
public:
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = { 0, 0 };
    int width;
    int height;
    int speed = 1;
    float offset;
};

#endif