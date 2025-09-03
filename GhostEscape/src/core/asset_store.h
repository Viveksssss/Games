#ifndef ASSET_STORE_H
#define ASSET_STORE_H

#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
class AssetStore {
public:
    AssetStore(SDL_Renderer* renderer);

    void loadTexture(const std::string& path);
    void loadMusic(const std::string& path);
    void loadSound(const std::string& path);
    void loadFont(const std::string& path, int size);

    SDL_Texture* getTexture(const std::string& path);
    Mix_Music* getMusic(const std::string& path);
    Mix_Chunk* getSound(const std::string& path);
    TTF_Font* getFont(const std::string& path, int size);

    void clean();
private:
    std::unordered_map<std::string, SDL_Texture*> textures;
    std::unordered_map<std::string, Mix_Music*> musics;
    std::unordered_map<std::string, Mix_Chunk*> sounds;
    std::unordered_map<std::string, TTF_Font*> fonts;
    SDL_Renderer* _renderer;
};

#endif