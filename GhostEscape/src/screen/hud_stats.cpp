#include "hud_stats.h"
#include "../affiliate/sprite.h"
#include "../core/actor.h"
#include "../raw/stats.h"

void HUDStats::init()
{
    ObjectScreen::init();
    _health_bg = Sprite::create(this, "assets/UI/bar_bg.png", 5.0f, Anchor::CENTER_LEFT);
    _health_bg->setOffset(_health_bg->getOffset() + glm::vec2(30.0f, 10.0f));
    _health_bar = Sprite::create(this, "assets/UI/bar_red.png", 5.0f, Anchor::CENTER_LEFT);
    _health_bar->setOffset(_health_bar->getOffset() + glm::vec2(30.0f, 10.0f));
    _health_icon = Sprite::create(this, "assets/UI/Red Potion.png", 1.0f, Anchor::CENTER_LEFT);
    _health_icon->setOffset(_health_icon->getOffset() + glm::vec2(5.0f, 10.0f));

    _mana_bg = Sprite::create(this, "assets/UI/bar_bg.png", 5.0f, Anchor::CENTER_LEFT);
    _mana_bg->setOffset(_mana_bg->getOffset() + glm::vec2(500.0f, 10.0f));
    _mana_bar = Sprite::create(this, "assets/UI/bar_blue.png", 5.0f, Anchor::CENTER_LEFT);
    _mana_bar->setOffset(_mana_bar->getOffset() + glm::vec2(500.0f, 10.0f));
    _health_icon = Sprite::create(this, "assets/UI/Blue Potion.png", 1.0f, Anchor::CENTER_LEFT);
    _health_icon->setOffset(_health_icon->getOffset() + glm::vec2(475.0f, 10.0f));
}

void HUDStats::render()
{
    ObjectScreen::render();
}

void HUDStats::update(float dt)
{
    ObjectScreen::update(dt);
    updateHealth();
    updateMana();
}

void HUDStats::updateHealth()
{
    if (!_target || !_health_bar || !_target->getStats()) {
        return;
    }
    _health_bar->setPercentage({ _target->getStats()->getHealth() / _target->getStats()->getMaxHealth(), 1.0f });
}

void HUDStats::updateMana()
{
    if (!_target || !_health_bar || !_target->getStats()) {
        return;
    }
    _mana_bar->setPercentage({ _target->getStats()->getMana() / _target->getStats()->getMaxMana(), 1.0f });
}

HUDStats* HUDStats::create(Object* parent, Actor* target, const glm::vec2& render_position)
{
    auto hud_stats = new HUDStats();
    hud_stats->init();
    hud_stats->setRenderPosition(render_position);
    hud_stats->setTarget(target);
    if (parent) {
        parent->addChild(hud_stats);
    }
    return hud_stats;
}
