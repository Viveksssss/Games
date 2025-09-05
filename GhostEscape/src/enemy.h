
#include "core/actor.h"
#include "player.h"

class Enemy : public Actor {
protected:
    enum class State {
        NORMAL,
        HURT,
        DIE
    };

    State _state = State::NORMAL;
    Player* _target = nullptr;
    SpriteAnim* _anim_normal = nullptr;
    SpriteAnim* _anim_hurt = nullptr;
    SpriteAnim* _anim_die = nullptr;
    SpriteAnim* _current = nullptr;

public:
    void update(float dt);
    void aim_target(Player* target);
    void setTarget(Player* target);
    Player* getTarget() const;
    void changeStates(State new_state);
    Enemy::State getStates() const;
    void checkStates();
    void remove();
    void attack();
    static Enemy* create(Object* parent, const glm::vec2& pos, Object* target = nullptr);

    void init();
};