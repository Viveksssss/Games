#pragma once

#include <SDL3/SDL_rect.h>
#include <optional>
#include <string>

namespace engine::render {

class Sprite final {
private:
    /* 纹理资源的标志符:路径 */
    std::string _texture_id;
    /* 源矩形 */
    std::optional<SDL_FRect> _source_rect;
    /* 是否水平翻转 */
    bool _is_filpped = false;

public:
    Sprite(const std::string& texture_id, const std::optional<SDL_FRect>& source_rect = std::nullopt, bool is_filpped = false)
        : _texture_id(texture_id)
        , _source_rect(source_rect)
        , _is_filpped(is_filpped)
    {
    }

    /* getter and setter */
    inline const std::string& getTextureId() const { return _texture_id; }
    inline const std::optional<SDL_FRect>& getSourceRect() const { return _source_rect; }
    inline bool isFlipped() const { return _is_filpped; }

    inline void setTextureId(const std::string& texture_id) { _texture_id = texture_id; }
    inline void setSourceRect(const std::optional<SDL_FRect>& source_rect) { _source_rect = source_rect; }
    inline void setFlipped(bool is_filpped) { _is_filpped = is_filpped; }
};

}
