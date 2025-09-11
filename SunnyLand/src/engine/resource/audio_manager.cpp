
#include "audio_manager.h"
#include <SDL3/SDL_error.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <spdlog/spdlog.h>

namespace engine::resource {
AudioManager::AudioManager()
{
    MIX_InitFlags flags = (MIX_INIT_OGG | MIX_INIT_MP3);
    if ((Mix_Init(flags) & flags) != flags) {
        throw std::runtime_error("Failed to initialize Mixer" + std::string(SDL_GetError()));
    }

    if (!Mix_OpenAudio(0, nullptr)) {
        Mix_Quit();
        throw std::runtime_error("Failed to open audio" + std::string(SDL_GetError()));
    }
    spdlog::debug("AudioManager initialized");
}

AudioManager::~AudioManager()
{
    Mix_HaltChannel(-1);
    Mix_HaltMusic();

    clearSounds();
    clearMusics();

    Mix_CloseAudio();
    Mix_Quit();
    spdlog::debug("AudioManager destroyed");
}

Mix_Chunk* AudioManager::loadSound(const std::string& path)
{
    auto it = _sounds.find(path);
    if (it != _sounds.end()) {
        return it->second.get();
    }
    spdlog::debug("Loading sound: {}", path);
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
        spdlog::error("Failed to load sound: {}", path);
        return nullptr;
    }

    _sounds.emplace(path, std::unique_ptr<Mix_Chunk, SDLMixChunkDeleter>(chunk));
    spdlog::debug("Sound loaded: {}", path);
    return chunk;
}

Mix_Chunk* AudioManager::getSound(const std::string& path)
{
    auto it = _sounds.find(path);
    if (it != _sounds.end()) {
        return it->second.get();
    }
    spdlog::warn("Sound not found: {},now try to load", path);
    return loadSound(path);
}

void AudioManager::unloadSound(const std::string& path)
{
    auto it = _sounds.find(path);
    if (it != _sounds.end()) {
        spdlog::debug("Unloading sound: {}", path);
        _sounds.erase(it);
    } else {
        spdlog::warn("Sound not found: {},can not to unload", path);
    }
}

void AudioManager::clearSounds()
{
    if (!_sounds.empty()) {
        spdlog::debug("Clearing sounds {}s", _sounds.size());
        _sounds.clear();
    }
}

Mix_Music* AudioManager::loadMusic(const std::string& path)
{
    auto it = _musics.find(path);
    if (it != _musics.end()) {
        return it->second.get();
    }
    spdlog::debug("Loading music: {}", path);
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music) {
        spdlog::error("Failed to load music: {}", path);
        return nullptr;
    }
    _musics.emplace(path, std::unique_ptr<Mix_Music, SDLMixMusicDeleter>(music));
    spdlog::debug("Music loaded: {}", path);
    return music;
}

Mix_Music* AudioManager::getMusic(const std::string& path)
{
    auto it = _musics.find(path);
    if (it != _musics.end()) {
        return it->second.get();
    }
    spdlog::warn("Music not found: {},now try to load", path);
    return loadMusic(path);
}

void AudioManager::unloadMusic(const std::string& path)
{
    auto it = _musics.find(path);
    if (it != _musics.end()) {
        spdlog::debug("Unloading music: {}", path);
        _musics.erase(it);
    } else {
        spdlog::warn("Music not found: {},can not to unload", path);
    }
}

void AudioManager::clearMusics()
{
    if (!_musics.empty()) {
        spdlog::debug("Clearing musics {}s", _musics.size());
        _musics.clear();
    }
}

void AudioManager::clearAudio()
{
    clearSounds();
    clearMusics();
}

}