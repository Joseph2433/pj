#include "WallNut.h"
#include "../Core/ResourceManager.h"
#include "../Systems/Grid.h"
#include "../Utils/Constants.h"
#include <iostream>

WallNut::WallNut(ResourceManager &resManager, const sf::Vector2i &gridPos, Grid &gridSystem)
    : Plant(resManager,
            WALLNUT_TEXTURE_KEY,
            gridPos,
            gridSystem,
            WALLNUT_HEALTH,
            WALLNUT_COST)
{
    std::cout << "WallNut created at (" << gridPos.x << "," << gridPos.y << ") with HP: " << m_health << std::endl;
}
