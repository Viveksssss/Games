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
    glm::vec2 _percentage = { 1.0f, 1.0f };

public:
    static Sprite* create(ObjectScreen* parent, const std::string& path, float scale = 1.0f, Anchor anchor = Anchor::CENTER);
    virtual ~Sprite() = default;
    virtual void setTexture(const Texture& texture);
    virtual Texture getTexture() const { return _texture; }
    virtual void render() override;

    virtual void setAngle(float angle) { _texture.angle = angle; }
    virtual float getAngle() const { return _texture.angle; }
    virtual void setFilp(bool is_flip) { _texture.is_filp = is_flip; }
    virtual bool getFilp() const { return _texture.is_filp; }
    virtual glm::vec2 getPercentage() const { return _percentage; }
    virtual void setPercentage(glm::vec2 percentage = { 1.0f, 1.0f }) { _percentage = percentage; }
};

#endif