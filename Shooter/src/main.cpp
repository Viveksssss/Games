#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>

#include <iostream>

#include "Game.h"

int main(int, char**)
{
    Game& game = Game::getInstance();
    game.init();
    game.run();
    return 0;
}