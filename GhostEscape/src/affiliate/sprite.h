#ifndef SPRITE_H
#define SPRITE_H

#include "../core/object_affiliate.h"

struct Texture {
    SDL_Texture* texture;
    SDL_FRect rect;
    float angle = 0;
    bool is_filp = false;
    Texture() = default;
    Texture(const std::string& path);
};

class Sprite : public ObjectAffiliate {
protected:
    Texture _texture;

public:
    static Sprite* create(ObjectScreen* parent, const std::string& path, float scale = 1.0f);
    virtual ~Sprite() = default;
    virtual void setTexture(const Texture& texture);
    virtual Texture getTexture() const { return _texture; };
    virtual void render() override;

    virtual void setAngle(float angle) { _texture.angle = angle; };
    virtual float getAngle() const { return _texture.angle; };
    virtual void setFilp(bool is_flip) { _texture.is_filp = is_flip; };
    virtual bool getFilp() const { return _texture.is_filp; };
};

#endif