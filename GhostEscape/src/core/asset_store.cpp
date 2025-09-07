#include "asset_store.h"
void AssetStore::loadTexture(const std::string& path)
{
    SDL_Texture* texture = IMG_LoadTexture(_renderer, path.c_str());
    if (texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", SDL_GetError());
        textures[path] = nullptr;
    } else {
        textures.emplace(path, texture);
    }
}

void AssetStore::loadMusic(const std::string& path)
{
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (music == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load music: %s", SDL_GetError());
        return;
    } else {
        musics.emplace(path, music);
    }
}

void AssetStore::loadSound(const std::string& path)
{
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load sound: %s", SDL_GetError());
        return;
    }
    sounds.emplace(path, sound);
}

void AssetStore::loadFont(const std::string& path, int size)
{
    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if (font == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font: %s ", SDL_GetError());
        return;
    }
    fonts.emplace(path + std::to_string(size), font);
}

SDL_Texture* AssetStore::getTexture(const std::string& path)
{
    auto it = textures.find(path);
    if (it == textures.end()) {
        loadTexture(path);
        it = textures.find(path);
    }       
    if (it == textures.end()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", SDL_GetError());
        return nullptr;
    }
    return it->second;
}

Mix_Music* AssetStore::getMusic(const std::string& path)
{
    auto it = musics.find(path);
    if (it == musics.end()) {
        loadMusic(path);
        it = musics.find(path);
    }
    if (it == musics.end()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load music: %s", SDL_GetError());
        return nullptr;
    }
    return it->second;
}

Mix_Chunk* AssetStore::getSound(const std::string& path)
{
    auto it = sounds.find(path);
    if (it == sounds.end()) {
        loadSound(path);
        it = sounds.find(path);
    }
    if (it == sounds.end()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load sound: %s", SDL_GetError());
        return nullptr;
    }
    return it->second;
}

TTF_Font* AssetStore::getFont(const std::string& path, int size)
{
    auto it = fonts.find(path+std::to_string(size));
    if (it == fonts.end()) {
        loadFont(path, size);
        it = fonts.find(path+std::to_string(size));
    }
    if (it == fonts.end()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font: %s", SDL_GetError());
        return nullptr;
    }
    return it->second;
}

void AssetStore::clean()
{
    for (auto& texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
    for (auto& music : musics) {
        Mix_FreeMusic(music.second);
    }
    musics.clear();
    for (auto& sound : sounds) {
        Mix_FreeChunk(sound.second);
    }
    sounds.clear();
    for (auto& font : fonts) {
        TTF_CloseFont(font.second);
    }
    fonts.clear();
}

AssetStore::AssetStore(SDL_Renderer* renderer)
{
    _renderer = renderer;
}
