#ifndef COLLIDER_H
#define COLLIDER_H

#include "../core/object_affiliate.h"
// #include "../core/object_world.h"

class ObjectWorld;
class Collider : public ObjectAffiliate {
protected:
private:
    enum class ColliderType {
        CIRCLE, // 以大小的x轴为直径
        RECTANGLE // 以大小的x轴为宽，y轴为高
    };
    ColliderType type = ColliderType::CIRCLE;

public:
    void render() override;
    static Collider* create(ObjectWorld* parent, glm::vec2 size, ColliderType type = ColliderType::CIRCLE, Anchor anchor = Anchor::CENTER);
    bool isColliding(const Collider* other) const;
    inline ColliderType getColliderType() const { return type; }
    inline void setColliderType(ColliderType type) { this->type = type; }
};

#endif // COLLIDER_H