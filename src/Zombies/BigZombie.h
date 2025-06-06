#pragma once

#include "../Entities/Zombie.h"
#include "../Utils/Constants.h"

class ResourceManager;

class BigZombie : public Zombie
{
public:
    BigZombie(ResourceManager &resManager, const sf::Vector2f &spawnPosition, Grid &grid);
    ~BigZombie() override = default;
};
