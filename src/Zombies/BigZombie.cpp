#include "BigZombie.h"
#include "../Utils/Constants.h"
#include "../Core/ResourceManager.h"

BigZombie::BigZombie(ResourceManager &resManager, const sf::Vector2f &spawnPosition, Grid &grid)
    : Zombie(resManager,
             BIG_ZOMBIE_TEXTURE_KEY,
             spawnPosition,
             BIG_ZOMBIE_HEALTH,
             BIG_ZOMBIE_SPEED,
             BIG_ZOMBIE_DAMAGE_PER_ATTACK,
             BIG_ZOMBIE_ATTACK_INTERVAL,
             grid)
{
}
