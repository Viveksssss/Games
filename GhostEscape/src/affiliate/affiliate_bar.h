#ifndef AFFILIATE_BAR_H
#define AFFILIATE_BAR_H

#include "../core/object_affiliate.h"

class AffiliateBar : public ObjectAffiliate {
protected:
    float _percentage = 1.0f;
    SDL_FColor _color_high = { 0, 1, 0, 1 };
    SDL_FColor _color_mid = { 1, 0.65, 0, 1 };
    SDL_FColor _color_low = { 1, 0, 0, 1 };

private:
public:
    inline float getPercentage() const { return _percentage; }
    inline void setPercentage(float percentage) { _percentage = percentage; }
    inline SDL_FColor getColorHigh() const { return _color_high; }
    inline void setColorHigh(SDL_FColor color) { _color_high = color; }
    inline SDL_FColor getColorMid() const { return _color_mid; }
    inline void setColorMid(SDL_FColor color) { _color_mid = color; }
    inline SDL_FColor getColorLow() const { return _color_low; }
    inline void setColorLow(SDL_FColor color) { _color_low = color; }

    virtual void render() override;
    static AffiliateBar* create(ObjectScreen* parent, const glm::vec2& size = { 0, 0 }, const glm::vec2& offset = { 0, 0 }, float percentage = 1.0f, Anchor anchor = Anchor::CENTER);
};

#endif // AFFILIATE_BAR_H