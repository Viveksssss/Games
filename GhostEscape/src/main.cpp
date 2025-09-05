#include "core/game.h"
int main()
{
    Game& game = Game::GetInstance();
    game.init("GhostEscape", 1920, 1080);
    game.run();
    return 0;
}