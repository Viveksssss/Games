#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <SDL3_mixer/SDL_mixer.h>

namespace engine::resource {

class AudioManager final {
    friend class ResourceManager;

private:
    /**
     * @brief 自定义的删除器，保证哈希表在释放的时候，可释放内存
     */
    struct SDLMixChunkDeleter {
        void operator()(Mix_Chunk* chunk)
        {
            if (chunk) {
                Mix_FreeChunk(chunk);
            }
        }
    };
    struct SDLMixMusicDeleter {
        void operator()(Mix_Music* music)
        {
            if (music) {
                Mix_FreeMusic(music);
            }
        }
    };

    // 音效存储
    std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, SDLMixChunkDeleter>> _sounds;
    // 音乐存储
    std::unordered_map<std::string, std::unique_ptr<Mix_Music, SDLMixMusicDeleter>> _musics;

public:
    AudioManager();
    ~AudioManager();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;

private:
    Mix_Chunk* loadSound(const std::string& path);
    Mix_Chunk* getSound(const std::string& path);
    void unloadSound(const std::string& path);
    void clearSounds();

    Mix_Music* loadMusic(const std::string& path);
    Mix_Music* getMusic(const std::string& path);
    void unloadMusic(const std::string& path);
    void clearMusics();

    void clearAudio();
};

};