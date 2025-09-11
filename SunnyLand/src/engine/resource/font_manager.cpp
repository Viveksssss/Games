#include "font_manager.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <spdlog/spdlog.h>

namespace engine::resource {
FontManager::FontManager()
{
    if (!TTF_WasInit() && !TTF_Init()) {
        throw std::runtime_error("Failed to initialize TTF");
    }
    spdlog::debug("FontManager initialized");
}

TTF_Font* FontManager::loadFont(const std::string& path, int ptsize)
{
    if (ptsize <= 0) {
        spdlog::error("Invalid font size: {}", ptsize);
        return nullptr;
    }
    FontKey key = { path, ptsize };
    auto it = _fonts.find(key);
    if (it != _fonts.end()) {
        return it->second.get();
    }
    TTF_Font* font = TTF_OpenFont(path.c_str(), ptsize);
    if (!font) {
        spdlog::error("Failed to load font: {}", path);
        return nullptr;
    }
    _fonts.emplace(key, std::unique_ptr<TTF_Font, SDLFontDeleter>(font));
    return font;
}

TTF_Font* FontManager::getFont(const std::string& path, int ptsize)
{
    if (ptsize <= 0) {
        spdlog::error("Invalid font size: {}", ptsize);
        return nullptr;
    }
    FontKey key = { path, ptsize };
    auto it = _fonts.find(key);
    if (it != _fonts.end()) {
        return it->second.get();
    }

    spdlog::warn("Font not found: {} ,now try to load", path);
    return loadFont(path, ptsize);
}

void FontManager::unloadFont(const std::string& path, int ptsize)
{
    FontKey key = { path, ptsize };
    auto it = _fonts.find(key);
    if (it != _fonts.end()) {
        spdlog::debug("Unloading font: {}", path);
        _fonts.erase(it);
    } else {
        spdlog::warn("Font not found: {}", path);
    }
}

void FontManager::clearFonts()
{
    if (!_fonts.empty()) {
        _fonts.clear();
    }
}

FontManager::~FontManager()
{
    clearFonts();
    TTF_Quit();
    spdlog::debug("FontManager destroyed");
}

}
