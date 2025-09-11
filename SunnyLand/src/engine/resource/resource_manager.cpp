#include "resource_manager.h"
#include "audio_manager.h"
#include "font_manager.h"
#include "texture_manager.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace engine::resource {
ResourceManager::ResourceManager(SDL_Renderer* renderer)
{
    _texture_manager = std::make_unique<TextureManager>(renderer);
    _audio_manager = std::make_unique<AudioManager>();
    _font_manager = std::make_unique<FontManager>();
}

SDL_Texture* ResourceManager::loadTexture(const std::string& file_path)
{
    return _texture_manager->loadTexture(file_path);
}

SDL_Texture* ResourceManager::getTexture(const std::string& file_path)
{
    return _texture_manager->getTexture(file_path);
}

void ResourceManager::unloadTexture(const std::string& file_path)
{
    _texture_manager->unloadTexture(file_path);
}

glm::vec2 ResourceManager::getTextureSize(const std::string& file_path)
{
    return _texture_manager->getTextureSize(file_path);
}

void ResourceManager::clearTextures()
{
    _texture_manager->clearTextures();
}

Mix_Chunk* ResourceManager::loadSound(const std::string& file_path)
{
    return _audio_manager->loadSound(file_path);
}

Mix_Chunk* ResourceManager::getSound(const std::string& file_path)
{
    return _audio_manager->getSound(file_path);
}

void ResourceManager::unloadSound(const std::string& file_path)
{
    _audio_manager->unloadSound(file_path);
}

void ResourceManager::clearSounds()
{
    _audio_manager->clearSounds();
}

Mix_Music* ResourceManager::loadMusic(const std::string& file_path)
{
    return _audio_manager->loadMusic(file_path);
}

Mix_Music* ResourceManager::getMusic(const std::string& file_path)
{
    return _audio_manager->getMusic(file_path);
}

void ResourceManager::unloadMusic(const std::string& file_path)
{
    _audio_manager->unloadMusic(file_path);
}

void ResourceManager::clearMusic()
{
    _audio_manager->clearMusics();
}

TTF_Font* ResourceManager::loadFont(const std::string& file_path, int point_size)
{
    return _font_manager->loadFont(file_path, point_size);
}

TTF_Font* ResourceManager::getFont(const std::string& file_path, int point_size)
{
    return _font_manager->getFont(file_path, point_size);
}

void ResourceManager::unloadFont(const std::string& file_path, int point_size)
{
    _font_manager->unloadFont(file_path, point_size);
}

void ResourceManager::clearFonts()
{
    _font_manager->clearFonts();
}

void ResourceManager::clear()
{
    _font_manager->clearFonts();
    _audio_manager->clearSounds();
    _texture_manager->clearTextures();

    spdlog::trace("ResourceManager 构造成功。");
}

ResourceManager::~ResourceManager() = default;

};