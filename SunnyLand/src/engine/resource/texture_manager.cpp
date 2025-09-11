#include "texture_manager.h"

#include <spdlog/spdlog.h>

namespace engine::resource {
TextureManager::TextureManager(SDL_Renderer* renderer)
    : _renderer(renderer)
{
    if (!_renderer) {
        throw std::runtime_error("Renderer is nullptr");
    }
    spdlog::debug("TextureManager initialized");
}

TextureManager::~TextureManager()
{
    clearTextures();
}

SDL_Texture* TextureManager::loadTexture(const std::string& file_path)
{
    auto it = _textures.find(file_path);
    if (it != _textures.end()) {
        return it->second.get();
    }
    SDL_Texture* texture = IMG_LoadTexture(_renderer, file_path.c_str());
    if (!texture) {
        spdlog::error("Failed to load texture: {}", file_path);
        return nullptr;
    }

    _textures.emplace(file_path, std::unique_ptr<SDL_Texture, SDLTextureDeleter>(texture));
    return texture;
}

SDL_Texture* TextureManager::getTexture(const std::string& file_path)
{
    auto it = _textures.find(file_path);
    if (it != _textures.end()) {
        return it->second.get();
    }
    spdlog::warn("Texture not found: {},now try to load", file_path);
    return loadTexture(file_path);
}
glm::vec2 TextureManager::getTextureSize(const std::string& file_path)
{
    SDL_Texture* texture = getTexture(file_path);
    if (!texture) {
        spdlog::error("Failed to get texture size: {}", file_path);
        return glm::vec2(0.0f);
    }
    glm::vec2 size;
    if (!SDL_GetTextureSize(texture, &size.x, &size.y)) {
        spdlog::error("Failed to get texture size: {}", file_path);
        return glm::vec2(0.0f);
    }
    return size;
}

void TextureManager::unloadTexture(const std::string& file_path)
{
    auto it = _textures.find(file_path);
    if (it != _textures.end()) {
        _textures.erase(it);
    } else {
        spdlog::warn("Texture not found: {}", file_path);
    }
}

void TextureManager::clearTextures()
{
    if (!_textures.empty()) {
        spdlog::debug("Clearing texture manager");
        _textures.clear();
    }
}
}