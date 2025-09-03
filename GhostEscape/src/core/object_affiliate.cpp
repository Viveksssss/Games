#include "object_affiliate.h"
void ObjectAffiliate::setOffsetByAnchor(const Anchor& anchor)
{
    _anchor = anchor;
    switch (anchor) {
    case Anchor::NONE:
        break;
    case Anchor::TOP_LEFT:
        _offset = glm::vec2(0, 0);
        break;
    case Anchor::TOP_CENTER:
        _offset = glm::vec2(-_size.x / 2, 0);
        break;
    case Anchor::TOP_RIGHT:
        _offset = glm::vec2(-_size.x, 0);
        break;
    case Anchor::CENTER_LEFT:
        _offset = glm::vec2(0, -_size.y / 2.0f);
        break;
    case Anchor::CENTER:
        _offset = glm::vec2(-_size.x / 2.0f, -_size.y / 2.0f);
        break;
    case Anchor::CENTER_RIGHT:
        _offset = glm::vec2(_size.x, _size.y / 2);
        break;
    case Anchor::BOTTOM_LEFT:
        _offset = glm::vec2(0, -_size.y);
        break;
    case Anchor::BOTTOM_CENTER:
        _offset = glm::vec2(-_size.x / 2, -_size.y);
        break;
    case Anchor::BOTTOM_RIGHT:
        _offset = glm::vec2(-_size.x, -_size.y);
        break;
    default:
        break;
    }
}
