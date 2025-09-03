#ifndef OBJECT_WORLD_H
#define OBJECT_WORLD_H

#include "object_screen.h"

class ObjectWorld : public ObjectScreen {
protected:
    // 世界坐标
    glm::vec2 world_position = glm::vec2(0.0f, 0.0f);

public:
    glm::vec2 getPosition() const;
    void setPosition(const glm::vec2& pos);
    virtual void setRenderPosition(const glm::vec2& pos) override;
    virtual void update(float dt) override;

    virtual void init() override { _type = ObjectType::OBJECT_WORLD; }
};

#endif