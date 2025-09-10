#include "bg_star.h"
#include "../core/scene.h"

BgStar* BgStar::create(Object* parent, int size, float scale_far, float scale_mid, float scale_near)
{
    BgStar* star = new BgStar();
    star->init();
    star->setNum(size);
    star->_scale_far = scale_far;
    star->_scale_mid = scale_mid;
    star->_scale_near = scale_near;

    star->_star_far.reserve(size);
    star->_star_mid.reserve(size);
    star->_star_near.reserve(size);
    auto extra = Game::GetInstance().getCurrentScene()->getWorldSize() - Game::GetInstance().getScreenSize();
    auto screen = Game::GetInstance().getScreenSize();
    for (int i = 0; i < size; ++i) {
        star->_star_far.push_back(Game::GetInstance().randomVec2(glm::vec2(0), screen + extra * scale_far));
        star->_star_mid.push_back(Game::GetInstance().randomVec2(glm::vec2(0), screen + extra * scale_mid));
        star->_star_near.push_back(Game::GetInstance().randomVec2(glm::vec2(0), screen + extra * scale_near));
    }
    if (parent) {
        parent->addChild(star);
    }
    return star;
}
void BgStar::update(float dt)
{
    timer += dt;
    _color_far = { 0.5f + 0.5f * std::sinf(timer * 0.9f), 0.5f + 0.5f * std::sinf(timer * 0.8f), 0.5f + 0.5f * std::sinf(timer * 0.7f), 1 };
    _color_mid = { 0.5f + 0.5f * std::sinf(timer * 0.7f), 0.5f + 0.5f * std::sinf(timer * 0.6f), 0.5f + 0.5f * std::sinf(timer * 0.5f), 1 };
    _color_near = { 0.5f + 0.5f * std::sinf(timer * 0.5f), 0.5f + 0.5f * std::sinf(timer * 0.4f), 0.5f + 0.5f * std::sinf(timer * 0.3f), 1 };
}

void BgStar::render()
{
    auto pos = -game.getCurrentScene()->getCameraPosition();
    game.renderPoints(_star_far, pos * _scale_far, _color_far);
    game.renderPoints(_star_mid, pos * _scale_mid, _color_mid);
    game.renderPoints(_star_near, pos * _scale_near, _color_near);
}
