#ifndef SCENE_END_H
#define SCENE_END_H

#include "Scene.h"
#include <SDL_mixer.h>
#include <fstream>
#include <map>
#include <string>

class SceneEnd : public Scene {
public:
    SceneEnd();
    SceneEnd(int _score);
    ~SceneEnd() = default;
    void init() override;
    void update() override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event* event) override;
    void updateTimer(float dt);
    void render_Text(SDL_Point p);
    void render_RankList();
    void insertScoreToRankList(int score, std::string name);

    void renderPhase1();
    void renderPhase2();

    void removeLastUTF8Char(std::string& str);

    void getFileData();
    void saveFileData();

private:
    bool isTyping = true;
    size_t score;
    std::string name;
    float timer = 0.0f;
    std::multimap<int, std::string, std::greater<int>> rankList;
    std::fstream file;
    Mix_Music* music;
};

#endif
