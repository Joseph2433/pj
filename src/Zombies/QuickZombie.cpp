#include "QuickZombie.h"
#include "../Utils/Constants.h"
#include "../Core/ResourceManager.h"

QuickZombie::QuickZombie(ResourceManager &resManager, const sf::Vector2f &spawnPosition, Grid &grid)
    : Zombie(resManager,
             QUICK_ZOMBIE_TEXTURE_KEY,
             spawnPosition,
             QUICK_ZOMBIE_HEALTH,
             QUICK_ZOMBIE_SPEED,
             QUICK_ZOMBIE_DAMAGE_PER_ATTACK,
             QUICK_ZOMBIE_ATTACK_INTERVAL,
             grid)
{
}
