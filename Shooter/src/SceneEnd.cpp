#include "SceneEnd.h"
#include "Game.h"
#include "Scene.h"
#include "SceneMain.h"
#include "TextRender.h"
#include "tool.h"
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_log.h>

#include <iostream>

SceneEnd::SceneEnd()

{
}

SceneEnd::SceneEnd(int _score)
    : score(_score)
{
    std::string path = getPath("src/Scores.txt");
    file.open(path, std::ios::in | std::ios::out);
}

void SceneEnd::init()
{
    getFileData();
    if (isTyping && !SDL_IsTextInputActive()) {
        SDL_StartTextInput();
    }

    music = Mix_LoadMUS(getPath("assets/music/06_Battle_in_Space_Intro.ogg").c_str());
    if (music) {
        Mix_PlayMusic(music, -1);
    }
}   

void SceneEnd::update()
{
    updateTimer(game.getDeltaTime());
}

void SceneEnd::render()
{
    if (isTyping) {
        renderPhase1();
    } else {
        renderPhase2();
    }
}

void SceneEnd::clean()
{
    for (auto it = rankList.begin(); it != rankList.end();) {
        it = rankList.erase(it);
    }
    rankList.clear();

    if(music!=nullptr){
        Mix_FreeMusic(music);
        music = nullptr;
    }
}

void SceneEnd::handleEvents(SDL_Event* event)
{
    if (isTyping) {
        if (event->type == SDL_TEXTINPUT) {
            name += event->text.text;
        }
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.sym == SDLK_RETURN) {
                isTyping = false;
                SDL_StopTextInput();
                if (name == "") {
                    name = "无名氏";
                }
                insertScoreToRankList(score, name);
            }
            if (event->key.keysym.sym == SDLK_BACKSPACE && name != "") {
                removeLastUTF8Char(name);
            }
        }
    } else {
        if (event->key.keysym.sym == SDLK_j) {
            saveFileData();
            game.changeScene(new SceneMain);
        }
    }
}

void SceneEnd::updateTimer(float dt)
{
    timer += dt;
    if (timer > 1.0f) {
        timer = 0.0f;
    }
}

void SceneEnd::render_Text(SDL_Point p)
{
    if (timer < 0.5f)
        game.textRenderer->renderText(game.getRenderer(), game.textFont, "_", p.x, p.y);
}

void SceneEnd::render_RankList()
{
    int i = 0;
    for (auto& [score, name] : rankList) {
        std::string fore = std::to_string(i + 1) + "." + name;
        std::string back = std::to_string(score);

        game.textRenderer->renderText(game.getRenderer(), game.textFont, fore, 120, 300 + i * 50);
        game.textRenderer->renderText(game.getRenderer(), game.textFont, back, 380, 300 + i * 50);
        i++;
    }
}

void SceneEnd::insertScoreToRankList(int score, std::string name)
{
    rankList.insert(std::make_pair(score, name));
    if (rankList.size() > 8) {
        rankList.erase(--rankList.end());
    }
}

void SceneEnd::renderPhase1()
{
    std::string scoreText = "Your Score:" + std::to_string(this->score);
    std::string text = "Game Over";
    std::string tip = "Input your name:";

    game.textRenderer->renderText(game.getRenderer(), game.textFont, scoreText, 200);
    game.textRenderer->renderText(game.getRenderer(), game.titleFont, text, 400);
    game.textRenderer->renderText(game.getRenderer(), tip, 190, 600);

    if (name != "") {
        SDL_Point p = game.textRenderer->renderText(game.getRenderer(), game.textFont, name, 700);
        render_Text(p);

    } else {
        game.textRenderer->renderText(game.getRenderer(), game.textFont, "_", 700);
    }
}

void SceneEnd::renderPhase2()
{
    std::string title = "Game Ranking";
    game.textRenderer->renderText(game.getRenderer(), game.titleFont, title, 200);

    render_RankList();

    std::string text = "Press J to restart Game";
    if (timer < 0.5f)
        game.textRenderer->renderText(game.getRenderer(), game.textFont, text, 800);
}

void SceneEnd::removeLastUTF8Char(std::string& str)
{
    // 中文的二进制：11xxxxxx 10xxxxxx 10xxxxxx
    // 英文字符的二进制：0xxxxxxx
    auto lastChar = str.back();
    if ((lastChar & 0b10000000) == 0b10000000) {
        str.pop_back();
        while ((str.back() & 0b11000000) != 0b11000000) {
            str.pop_back();
        }
    }
    str.pop_back();
}

void SceneEnd::getFileData()
{
    if (!file.is_open()) {
        file.open(getPath("src/Scores.txt"), std::ios::out | std::ios::in);
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find(":");
        int score = std::stoi(line.substr(0, pos));
        std::string name = line.substr(pos + 1);
        insertScoreToRankList(score, name);
    }
    file.clear();
    file.seekg(0);
}

void SceneEnd::saveFileData()
{
    // 先确保排行榜大小不超过8
    while (rankList.size() > 8) {
        rankList.erase(--rankList.end());
    }

    file.close();
    file.clear();
    file.open(getPath("src/Scores.txt"), std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing!" << std::endl;
        return;
    }

    for (auto& [score, name] : rankList) {
        file << std::to_string(score) << ":" << name << std::endl;
    }

    file.flush();
    file.close();
    file.clear();
}
