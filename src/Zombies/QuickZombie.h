#pragma once

#include "../Entities/Zombie.h"
#include "../Utils/Constants.h"

class ResourceManager;

class QuickZombie : public Zombie
{
public:
    QuickZombie(ResourceManager &resManager, const sf::Vector2f &spawnPosition, Grid &grid);
    ~QuickZombie() override = default;
};
