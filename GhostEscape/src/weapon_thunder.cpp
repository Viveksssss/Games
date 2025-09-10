#include "weapon_thunder.h"
#include "core/actor.h"
#include "core/scene.h"
#include "screen/hud_skill.h"
#include "world/spell.h"

bool WeaponThunder::handleEvents(SDL_Event& event)
{
    Weapon::handleEvents(event);
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            if (canAttack()) {
                game.playSound("assets/sound/big-thunder.mp3");
                auto pos = game.getCurrentScene()->screenToWorld({ event.button.x, event.button.y });
                auto spell = Spell::create(nullptr, "assets/effect/Thunderstrike w blur.png", pos, 45.0f, 10.0f, Anchor::CENTER);
                attack(pos, spell);
                return true;
            }
        }
    }
    return false;
}

WeaponThunder* WeaponThunder::create(Actor* parent, float cool_down, float mana_cost)
{
    auto weapon = new WeaponThunder();
    weapon->init();
    weapon->set_cool_down(cool_down);
    weapon->set_mana_cost(mana_cost);
    weapon->setParent(parent);
    if (parent) {
        parent->safeAddChild(weapon);
    }
    return weapon;
}

void WeaponThunder::init()
{
    Weapon::init();
    auto pos = glm::vec2(game.getScreenSize().x - 500, 50);
    _hud_skill = HUDSkill::create(game.getCurrentScene(), "assets/UI/Electric-Icon.png", pos, 0.3f, Anchor::CENTER);
}

void WeaponThunder::update(float delta)
{
    Weapon::update(delta);
    if (_hud_skill) {
        _hud_skill->setPercentage(_cool_down_timer / _cool_down);
    }
}
