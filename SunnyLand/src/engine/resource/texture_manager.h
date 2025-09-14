#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include <SDL3_image/SDL_image.h>

namespace engine::resource {

class TextureManager {
    friend class ResourceManager;

private:
    struct SDLTextureDeleter {
        void operator()(SDL_Texture* texture) const
        {
            if (texture) {
                SDL_DestroyTexture(texture);
            }
        }
    };

    std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> _textures;
    SDL_Renderer* _renderer;

public:
    explicit TextureManager(SDL_Renderer* renderer);
    ~TextureManager();

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager(std::string&& filename) = delete;
    TextureManager& operator=(std::string&& filename) = delete;

private:
    SDL_Texture* loadTexture(const std::string& file_path);
    SDL_Texture* getTexture(const std::string& file_path);
    glm::vec2 getTextureSize(const std::string& file_path);
    void unloadTexture(const std::string& file_path);
    void clearTextures();
};

};