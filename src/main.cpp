// src/main.cpp
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
        std::cerr << "Exception caught in main: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception caught in main." << std::endl;
        return 1;
    }
    return 0;
}
