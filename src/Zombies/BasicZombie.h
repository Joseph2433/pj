#pragma once

#include "../Entities/Zombie.h"
#include "../Utils/Constants.h"

class ResourceManager;

class BasicZombie : public Zombie
{
public:
    BasicZombie(ResourceManager &resManager, const sf::Vector2f &spawnPosition, Grid &grid);
    ~BasicZombie() override = default;
};
