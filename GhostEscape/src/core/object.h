#ifndef OBJECT_H
#define OBJECT_H

#include "defs.h"
#include "game.h"
#include <vector>
class Object {
protected:
    ObjectType _type = ObjectType::NONE;
    Game& game = Game::GetInstance();
    std::vector<Object*> _children;
    std::vector<Object*> _object_to_add;
    bool is_active = true;
    bool need_remove = false;

private:
public:
    Object() = default;
    virtual ~Object() = default;
    virtual void init() { };
    virtual void handleEvents(SDL_Event& event);
    virtual void update([[maybe_unused]] float dt);
    virtual void render();
    virtual void clean();

    virtual void addChild(Object* child);
    virtual void safeAddChild(Object* child);
    virtual void removeChild(Object* child);

    virtual ObjectType getType() { return _type; }
    virtual void setType(const ObjectType& type) { this->_type = type; }

    virtual bool isActive() const { return is_active; }
    virtual void setActive(bool active) { is_active = active; }
    virtual bool getNeedRemove() const { return need_remove; }
    virtual void setNeedRemove(bool remove) { need_remove = remove; }
};

#endif