#include "SceneTitle.h"
#include "Game.h"
#include "SceneMain.h"
#include "tool.h"
#include "TextRender.h"

#include <string>
void SceneTitle::init()
{
    if ((music = Mix_LoadMUS(getPath("assets/music/06_Battle_in_Space_Intro.ogg").c_str())) == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%d:SDL_Error::%s\n", __LINE__, SDL_GetError());
    }
    Mix_PlayMusic(music, -1);
}

void SceneTitle::clean()
{
    if(music){
        Mix_FreeMusic(music);
        music = nullptr;
    }
}

void SceneTitle::update()
{
    timer += game.getDeltaTime();
    if (timer >= 1.0f) {
        timer = 0.0f;
    }
}

void SceneTitle::render()
{
    std::string title = "Space Invaders";
    game.textRenderer->renderText(game.getRenderer(), game.titleFont, title, 300);

    std::string text = "Press J To Start Game";
    if (timer < 0.5f) {
        game.textRenderer->renderText(game.getRenderer(), game.textFont, text, 600);
    }
}

void SceneTitle::handleEvents(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_j) {
        game.changeScene(new SceneMain);
    }
}

SceneTitle::SceneTitle()
{
}
