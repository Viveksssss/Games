#include "weapon_thunder.h"
#include "core/actor.h"
#include "core/scene.h"
#include "world/spell.h"

void WeaponThunder::handleEvents(SDL_Event& event)
{
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            if (canAttack()) {
                auto pos = game.getCurrentScene()->screenToWorld({ event.button.x, event.button.y });
                auto spell = Spell::create(nullptr, "assets/effect/Thunderstrike w blur.png", pos, 45.0f, 10.0f, Anchor::CENTER);
                attack(pos, spell);
            }
        }
    }
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
