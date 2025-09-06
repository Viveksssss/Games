#include "affiliate_bar.h"
void AffiliateBar::render()
{
    ObjectAffiliate::render();
    auto pos = _parent->getRenderPosition() + _offset;
    if (_percentage > 0.7) {
        game.renderHBar(pos, _size, _percentage, _color_high);
    } else if (_percentage > 0.3) {
        game.renderHBar(pos, _size, _percentage, _color_mid);
    } else {
        game.renderHBar(pos, _size, _percentage, _color_low);
    }
}

AffiliateBar* AffiliateBar::create(ObjectScreen* parent, const glm::vec2& size, const glm::vec2& offset, float percentage, Anchor anchor)
{
    auto bar = new AffiliateBar();
    bar->init();
    bar->setOffset(offset);
    bar->setOffsetByAnchor(anchor);
    bar->setSize(size);
    bar->setPercentage(percentage);
    if (parent) {
        bar->setParent(parent);
        parent->addChild(bar);
    }
    return bar;
}
