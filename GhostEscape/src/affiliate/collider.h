#ifndef COLLIDER_H
#define COLLIDER_H

#include "../core/object_affiliate.h"

class Collider : public ObjectAffiliate {
protected:
private:
    enum class Type {
        CIRCLE, // 以大小的x轴为半径
        RECTANGLE // 以大小的x轴为宽，y轴为高
    };
    Type type = Type::RECTANGLE;

public:
};

#endif // COLLIDER_H