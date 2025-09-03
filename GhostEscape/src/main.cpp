#include "core/game.h"
int main()
{
    Game&game = Game::GetInstance();
    game.init("GhostEscape",1280,720);
    game.run();
    return 0;
}