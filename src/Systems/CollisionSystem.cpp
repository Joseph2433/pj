#include "CollisionSystem.h"
#include "../Entities/Projectile.h" // 包含完整的 Projectile 定义
#include "../Entities/Zombie.h"     // 包含完整的 Zombie 定义
#include "../Entities/Plant.h"      // 包含完整的 Plant 定义
#include "ProjectileManager.h"      // 包含 ProjectileManager 以使用其接口
#include "ZombieManager.h"          // 包含 ZombieManager
#include "PlantManager.h"           // 包含 PlantManager
#include "../Utils/Constants.h"     // 可能需要攻击范围等常量
#include <iostream>                 // 用于调试

CollisionSystem::CollisionSystem()
{
}

void CollisionSystem::update(ProjectileManager &projectileManager,
                             ZombieManager &zombieManager,
                             PlantManager &plantManager)
{
    std::vector<Projectile *> activeProjectiles = projectileManager.getAllActiveProjectiles();
    std::vector<Zombie *> activeZombies = zombieManager.getActiveZombies();
    // std::vector<Plant*> activePlants = plantManager.getAllActivePlants(); // 如果需要植物间碰撞

    checkProjectileZombieCollisions(activeProjectiles, activeZombies);
}

void CollisionSystem::checkProjectileZombieCollisions(std::vector<Projectile *> &projectiles,
                                                      std::vector<Zombie *> &zombies)
{
    for (Projectile *projectile : projectiles)
    {
        // if (!projectile || projectile->hasHit()) continue; // getAllActiveProjectiles 应该已经过滤了hasHit的
        if (!projectile)
            continue; // 安全检查

        for (Zombie *zombie : zombies)
        {
            // if (!zombie || !zombie->isAlive()) continue; // getActiveZombies 应该已经过滤了
            if (!zombie)
                continue;

            // 简单的矩形碰撞检测
            if (projectile->getGlobalBounds().intersects(zombie->getGlobalBounds()))
            {
                // 确保子弹和僵尸在同一行（或Y坐标足够接近）才算有效击中
                float projYCenter = projectile->getPosition().y;
                float zombieBodyTop = zombie->getPosition().y - zombie->getGlobalBounds().height - 15; // 假设原点在底部
                float zombieBodyBottom = zombie->getPosition().y + 15;

                if (projYCenter > zombieBodyTop && projYCenter < zombieBodyBottom)
                { // 子弹Y在僵尸身体范围内
                    std::cout << "[CollisionSystem] DEBUG: 子弹 (Addr: " << projectile
                              << ") 击中僵尸 (Addr: " << zombie << ")" << std::endl;

                    zombie->takeDamage(projectile->getDamage()); // 僵尸受到伤害
                    projectile->onHit();                         // 标记子弹已击中 (它将在 ProjectileManager::update 中被移除)

                    break; // 一个子弹只击中一个僵尸
                }
            }
        }
        if (projectile->hasHit())
        {
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