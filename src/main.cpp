#include "common.hpp"
#include "gamemanager.hpp"

int main(int argc, char** argv)
{
    Mario::GameManager game;

    game.Init();
    game.Loop();
    game.Cleanup();

    return 0;
}