#ifndef OBJECT_SCREEN_H
#define OBJECT_SCREEN_H

#include "object.h"
class ObjectScreen : public Object {
protected:
    // 渲染屏幕坐标
    glm::vec2 render_position = glm::vec2(0, 0);

public:
    glm::vec2 getRenderPosition() const { return render_position; }
    virtual void setRenderPosition(const glm::vec2& pos) { render_position = pos; }
    virtual void init() override { _type = ObjectType::OBJECT_SCREEN; }
};

#endif