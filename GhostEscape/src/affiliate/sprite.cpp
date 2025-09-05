#include "sprite.h"
#include "../core/asset_store.h"
#include <SDL3/SDL_log.h>
Texture::Texture(const std::string& path)
{
    this->texture = Game::GetInstance().getAssetStore()->getTexture(path);
    SDL_GetTextureSize(texture, &rect.w, &rect.h);
}

Sprite* Sprite::create(ObjectScreen* parent, const std::string& path, float scale, Anchor anchor)
{
    auto sprite = new Sprite();
    sprite->init();
    sprite->setTexture(Texture(path));
    sprite->setOffsetByAnchor(anchor);
    sprite->setParent(parent);
    sprite->setOffset(glm::vec2(0, 0));
    sprite->setScale(scale);
    if (parent)
        parent->addChild(sprite);
    return sprite;
}

void Sprite::setTexture(const Texture& texture)
{
    this->_texture = texture;
    _size = glm::vec2(texture.rect.w, texture.rect.h);
}

void Sprite::render()
{
    if (!_texture.texture || !_parent) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Texture is null : %d", __LINE__);
        return;
    }

    auto pos = _parent->getRenderPosition() + _offset;
    game.renderTexture(_texture, pos, _size);
}
