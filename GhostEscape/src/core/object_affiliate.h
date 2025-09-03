#ifndef OBJECT_AFFILIATE_H
#define OBJECT_AFFILIATE_H

#include "object.h"
#include "object_screen.h"
#include <glm/glm.hpp>

class ObjectAffiliate : public Object {
protected:
    ObjectScreen* _parent = nullptr;
    glm::vec2 _offset = glm::vec2(0);
    glm::vec2 _size = glm::vec2(0);
    float _scale = 1.0f;
    Anchor _anchor = Anchor::CENTER;

public:
    void setOffsetByAnchor(const Anchor& anchor);
    inline void setParent(ObjectScreen* parent) { _parent = parent; }
    inline void setOffset(const glm::vec2& offset) { _offset = offset; }
    inline void setSize(const glm::vec2& size) { _size = size; setOffsetByAnchor(_anchor); }
    inline void setScale(float scale) { _scale = scale, _size *= scale;setOffsetByAnchor(_anchor); }
    inline glm::vec2 getOffset() const { return _offset; }
    inline glm::vec2 getSize() const { return _size; }
    inline ObjectScreen* getParent() const { return _parent; }
    inline float getScale() const { return _scale; }
};

#endif