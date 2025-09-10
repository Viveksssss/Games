#include "asset_store.h"
void AssetStore::loadTexture(const std::string& path)
{
    SDL_Texture* texture = IMG_LoadTexture(_renderer, path.c_str());
    if (texture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", SDL_GetError());
        return;
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
    if (it == textures.end() || it->second == nullptr) {
        // 如果不存在或者是nullptr，都重新加载
        loadTexture(path);
        it = textures.find(path);
    }
    if (it == textures.end() || it->second == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", path.c_str());
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
    auto it = fonts.find(path + std::to_string(size));
    if (it == fonts.end()) {
        loadFont(path, size);
        it = fonts.find(path + std::to_string(size));
    }
    if (it == fonts.end()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font: %s", SDL_GetError());
        return nullptr;
    }
    return it->second;
}

void AssetStore::init()
{
    textures.emplace("assets/UI/29.png", IMG_LoadTexture(_renderer, "assets/UI/29.png"));
    textures.emplace("assets/UI/30.png", IMG_LoadTexture(_renderer, "assets/UI/30.png"));
    textures.emplace("assets/UI/bar_bg.png", IMG_LoadTexture(_renderer, "assets/UI/bar_bg.png"));
    textures.emplace("assets/UI/bar_blue.png", IMG_LoadTexture(_renderer, "assets/UI/bar_blue.png"));
    textures.emplace("assets/UI/bar_green.png", IMG_LoadTexture(_renderer, "assets/UI/bar_green.png"));
    textures.emplace("assets/UI/bar_red.png", IMG_LoadTexture(_renderer, "assets/UI/bar_red.png"));
    textures.emplace("assets/UI/A_Back1.png", IMG_LoadTexture(_renderer, "assets/UI/A_Back1.png"));
    textures.emplace("assets/UI/A_Back2.png", IMG_LoadTexture(_renderer, "assets/UI/A_Back2.png"));
    textures.emplace("assets/UI/A_Back3.png", IMG_LoadTexture(_renderer, "assets/UI/A_Back3.png"));
    textures.emplace("assets/UI/A_Credits1.png", IMG_LoadTexture(_renderer, "assets/UI/A_Credits1.png"));
    textures.emplace("assets/UI/A_Credits2.png", IMG_LoadTexture(_renderer, "assets/UI/A_Credits2.png"));
    textures.emplace("assets/UI/A_Credits3.png", IMG_LoadTexture(_renderer, "assets/UI/A_Credits3.png"));
    textures.emplace("assets/UI/A_Pause1.png", IMG_LoadTexture(_renderer, "assets/UI/A_Pause1.png"));
    textures.emplace("assets/UI/A_Pause2.png", IMG_LoadTexture(_renderer, "assets/UI/A_Pause2.png"));
    textures.emplace("assets/UI/A_Pause3.png", IMG_LoadTexture(_renderer, "assets/UI/A_Pause3.png"));
    textures.emplace("assets/UI/A_Quit1.png", IMG_LoadTexture(_renderer, "assets/UI/A_Quit1.png"));
    textures.emplace("assets/UI/A_Quit2.png", IMG_LoadTexture(_renderer, "assets/UI/A_Quit2.png"));
    textures.emplace("assets/UI/A_Quit3.png", IMG_LoadTexture(_renderer, "assets/UI/A_Quit3.png"));
    textures.emplace("assets/UI/A_Restart1.png", IMG_LoadTexture(_renderer, "assets/UI/A_Restart1.png"));
    textures.emplace("assets/UI/A_Restart2.png", IMG_LoadTexture(_renderer, "assets/UI/A_Restart2.png"));
    textures.emplace("assets/UI/A_Restart3.png", IMG_LoadTexture(_renderer, "assets/UI/A_Restart3.png"));
    textures.emplace("assets/UI/A_Start1.png", IMG_LoadTexture(_renderer, "assets/UI/A_Start1.png"));
    textures.emplace("assets/UI/A_Start2.png", IMG_LoadTexture(_renderer, "assets/UI/A_Start2.png"));
    textures.emplace("assets/UI/A_Start3.png", IMG_LoadTexture(_renderer, "assets/UI/A_Start3.png"));
    textures.emplace("assets/UI/Blue Potion.png", IMG_LoadTexture(_renderer, "assets/UI/Blue Potion.png"));
    textures.emplace("assets/UI/Electric-Icon.png", IMG_LoadTexture(_renderer, "assets/UI/Electric-Icon.png"));
    textures.emplace("assets/UI/Green Potion.png", IMG_LoadTexture(_renderer, "assets/UI/Green Potion.png"));
    textures.emplace("assets/UI/Red Potion.png", IMG_LoadTexture(_renderer, "assets/UI/Red Potion.png"));
    textures.emplace("assets/UI/Textfield_01.png", IMG_LoadTexture(_renderer, "assets/UI/Textfield_01.png"));
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
