#include "BasicZombie.h"
#include "../Utils/Constants.h"
#include "../Core/ResourceManager.h"

BasicZombie::BasicZombie(ResourceManager &resManager, const sf::Vector2f &spawnPosition, Grid &grid) // <--- 接收 Grid& grid
    : Zombie(resManager,
             BASIC_ZOMBIE_TEXTURE_KEY,
             spawnPosition,
             BASIC_ZOMBIE_HEALTH,
             BASIC_ZOMBIE_SPEED,
             BASIC_ZOMBIE_DAMAGE_PER_ATTACK,
             BASIC_ZOMBIE_ATTACK_INTERVAL,
             grid)
{
}
