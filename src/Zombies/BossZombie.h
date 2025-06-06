#pragma once

#include "../Entities/Zombie.h"
#include "../Utils/Constants.h"

class ResourceManager;

class BossZombie : public Zombie
{
public:
    BossZombie(ResourceManager &resManager, const sf::Vector2f &spawnPosition, Grid &grid);
    ~BossZombie() override = default;
};
