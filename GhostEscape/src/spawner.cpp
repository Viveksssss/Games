#include "spawner.h"
#include "core/scene.h"
#include "enemy.h"
#include "world/effect.h"

void Spawner::update(float dt)
{
    if (player && player->isActive()) {
        _timer += dt;
        if (_timer >= _interval) {
            _timer = 0;
            auto p1 = game.getCurrentScene()->getCameraPosition();
            auto p2 = game.getCurrentScene()->getCameraPosition() + game.getScreenSize();
            for (int i = 0; i < _num; ++i) {
                auto pos = game.randomVec2(p1, p2);
                Enemy* enemy = Enemy::create(nullptr, pos, this->player);
                Effect::create(game.getCurrentScene(), "assets/effect/184_3.png", pos, 1.0f, enemy);
            }
        }
    }
}
