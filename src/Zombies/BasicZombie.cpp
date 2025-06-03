#include "BasicZombie.h"
#include "../Core/ResourceManager.h" // 确保包含

BasicZombie::BasicZombie(ResourceManager &resManager, const sf::Vector2f &spawnPosition)
    : Zombie(resManager,               // 资源管理器
             BASIC_ZOMBIE_TEXTURE_KEY, // 纹理键名 (来自常量)
             spawnPosition,            // 生成位置
             BASIC_ZOMBIE_HEALTH,      // 生命值 (来自常量)
             BASIC_ZOMBIE_SPEED,       // 速度 (来自常量)
             BASIC_ZOMBIE_DAMAGE_PER_ATTACK)
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
