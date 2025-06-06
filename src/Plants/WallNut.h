#pragma once
#include "../Entities/Plant.h"

class ResourceManager;
class Grid;

class WallNut : public Plant
{
public:
    WallNut(ResourceManager &resManager, const sf::Vector2i &gridPos, Grid &gridSystem);
    ~WallNut() override = default;
};
