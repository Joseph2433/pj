#include "CollisionSystem.h"
#include "../Entities/Projectile.h"
#include "../Entities/Zombie.h"
#include "../Entities/Plant.h"
#include "ProjectileManager.h"
#include "ZombieManager.h"
#include "PlantManager.h"
#include "../Utils/Constants.h"
#include <iostream>

CollisionSystem::CollisionSystem()
{
}

void CollisionSystem::update(ProjectileManager &projectileManager,
                             ZombieManager &zombieManager,
                             PlantManager &plantManager)
{
    std::vector<Projectile *> activeProjectiles = projectileManager.getAllActiveProjectiles();
    std::vector<Zombie *> activeZombies = zombieManager.getActiveZombies();
    std::vector<Plant *> activePlants = plantManager.getAllActivePlants();

    checkProjectileZombieCollisions(activeProjectiles, activeZombies);
    checkZombiePlantCollisions(activeZombies, activePlants, 0.f);
}

void CollisionSystem::checkProjectileZombieCollisions(std::vector<Projectile *> &projectiles,
                                                      std::vector<Zombie *> &zombies)
{
    for (Projectile *projectile : projectiles)
    {
        if (!projectile || projectile->hasHit())
        {
            continue;
        }
        for (Zombie *zombie : zombies)
        {
            if (!zombie || !zombie->isAlive())
            {
                continue;
            }

            if (projectile->getGlobalBounds().intersects(zombie->getGlobalBounds()))
            {

                float projYCenter = projectile->getPosition().y;

                float zombieSpriteHeight = zombie->getGlobalBounds().height;
                float zombieFeetY = zombie->getPosition().y;
                float zombieHeadY = zombieFeetY - zombieSpriteHeight;

                // 允许一点容差，例如子弹高度的一半
                float yTolerance = projectile->getGlobalBounds().height / 2.f; // 或一个固定值

                //
                if (projYCenter >= zombieHeadY - yTolerance && projYCenter <= zombieFeetY + yTolerance)
                {
                    std::cout << "[CollisionSystem] Projectile (Addr: " << projectile
                              << ") potentially hit Zombie (Addr: " << zombie << ") on the same visual plane." << std::endl;

                    // --- 碰撞发生 ---
                    zombie->takeDamage(projectile->getDamage());
                    std::cout << "[CollisionSystem] Zombie (Addr: " << zombie << ") took "
                              << projectile->getDamage() << " damage. HP left: " << zombie->getHealth() << std::endl;
                    projectile->applyPrimaryEffect(zombie);
                    projectile->onHit();
                    break;
                }
                else
                {
                    std::cout << "[CollisionSystem] Projectile and Zombie intersect on X but not on Y plane." << std::endl;
                }
            }
        }
    }
}

// 检测僵尸与植物的碰撞
void CollisionSystem::checkZombiePlantCollisions(std::vector<Zombie *> &zombies,
                                                 std::vector<Plant *> &plants,
                                                 float dt)
{
    for (Zombie *zombie : zombies)
    {
        if (!zombie || !zombie->isAlive() || zombie->getCurrentState() == ZombieState::ATTACKING)
        {
            // 如果僵尸无效、死亡或已经在攻击状态，则跳过
            continue;
        }
    }
    (void)dt;
}