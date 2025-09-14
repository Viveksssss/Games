#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

namespace engine::resource {

using FontKey = std::pair<std::string, int>;

struct FontKeyHash {
    std::size_t operator()(const FontKey& key)const
    {
        std::hash<std::string> str_hasher;
        std::hash<int> int_hasher;
        return str_hasher(key.first) ^ int_hasher(key.second);
    }
};

/**
 * @brief 管理字体资源
 */
class FontManager {
    friend class ResourceManager;

private:
    /**
     * @brief 自定义删除器
     */
    struct SDLFontDeleter {
        void operator()(TTF_Font* font)
        {
            if (font) {
                TTF_CloseFont(font);
            }
        }
    };

    std::unordered_map<FontKey, std::unique_ptr<TTF_Font, SDLFontDeleter>, FontKeyHash> _fonts;

public:
    FontManager();

    ~FontManager();

    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
    FontManager(FontManager&&) = delete;
    FontManager& operator=(FontManager&&) = delete;

private:
    TTF_Font* loadFont(const std::string& path, int ptsize);
    TTF_Font* getFont(const std::string& path, int ptsize);
    void unloadFont(const std::string& path, int ptsize);
    void clearFonts();
};

}