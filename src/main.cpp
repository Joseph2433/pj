#include "Core/Game.h"
#include <iostream>

int main()
{
    try
    {
        Game game;
        game.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Game crashed with exception: " << e.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cerr << "Game crashed with unknown exception" << std::endl;
        return -1;
    }

    return 0;
}