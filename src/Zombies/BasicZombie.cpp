#include "BasicZombie.h"
#include "../Utils/Constants.h"
#include "../Core/ResourceManager.h" // 确保包含

BasicZombie::BasicZombie(ResourceManager &resManager, const sf::Vector2f &spawnPosition, Grid &grid) // <--- 接收 Grid& grid
    : Zombie(resManager,
             BASIC_ZOMBIE_TEXTURE_KEY,
             spawnPosition,
             BASIC_ZOMBIE_HEALTH,
             BASIC_ZOMBIE_SPEED,
             BASIC_ZOMBIE_DAMAGE_PER_ATTACK,
             BASIC_ZOMBIE_ATTACK_INTERVAL,
             grid)
{ // 伤害值 (来自常量)

    // 普通僵尸特有的初始化代码（如果需要）
    // 例如，如果普通僵尸有特定的动画设置
    // std::cout << "普通僵尸创建于 (" << spawnPosition.x << ", " << spawnPosition.y << ")" << std::endl;
}

// 如果 BasicZombie 没有特殊的更新逻辑 (例如，行走和攻击行为与 Zombie 基类一致)，
// 则不需要重写 update 方法。它会自动调用 Zombie::update()。
// void BasicZombie::update(float dt /*, const std::vector<Plant*>& plantsInLane */) {
//     Zombie::update(dt /*, plantsInLane */); // 调用基类更新
//     // BasicZombie 特有的更新逻辑 (如果除了基类行为外还有其他)
// }
