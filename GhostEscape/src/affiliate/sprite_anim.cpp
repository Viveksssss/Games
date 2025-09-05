#include "sprite_anim.h"

void SpriteAnim::setTexture(const Texture& texture)
{
    _texture = texture;
    _total_frame = _texture.rect.w / _texture.rect.h;
    _texture.rect.w = _texture.rect.h;
    _size = glm::vec2(_texture.rect.w, _texture.rect.h);
}

SpriteAnim* SpriteAnim::create(ObjectScreen* parent, const std::string& path, float scale, Anchor anchor)
{
    auto sprite = new SpriteAnim();
    sprite->init();
    sprite->setTexture(Texture(path));
    sprite->setOffset(glm::vec2(0, 0));
    sprite->setScale(scale);
    if (parent != nullptr) {
        parent->addChild(sprite);
        sprite->setParent(parent);
    }
    return sprite;
}

void SpriteAnim::update(float dt)
{
    if (is_finish) {
        return;
    }
    _frame_timer += dt;
    if (_frame_timer >= 1.0f / _fps) {
        _current_frame++;
        if (_current_frame >= _total_frame) {
            if (!is_loop) {
                is_finish = true;
            }
            _current_frame = 0;
        }
        _frame_timer = 0.0f;
    }
    _texture.rect.x = _texture.rect.w * _current_frame;
}

void SpriteAnim::render()
{
    if (is_finish) {
        return;
    }
    Sprite::render();
}
