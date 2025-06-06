#include "BossZombie.h"
#include "../Utils/Constants.h"
#include "../Core/ResourceManager.h"

BossZombie::BossZombie(ResourceManager &resManager, const sf::Vector2f &spawnPosition, Grid &grid) // <--- 接收 Grid& grid
    : Zombie(resManager,
             BOSS_ZOMBIE_TEXTURE_KEY,
             spawnPosition,
             BOSS_ZOMBIE_HEALTH,
             BOSS_ZOMBIE_SPEED,
             BOSS_ZOMBIE_DAMAGE_PER_ATTACK,
             BOSS_ZOMBIE_ATTACK_INTERVAL,
             grid)
{
}
