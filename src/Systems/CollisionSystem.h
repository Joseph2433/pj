#pragma once

#include <vector>
#include <memory> // For std::unique_ptr if needed, though usually works with raw pointers for a frame

// 前向声明，避免在头文件中包含完整的类定义，减少编译依赖
class Projectile;        // 子弹基类
class Zombie;            // 僵尸基类
class Plant;             // 植物基类
class ProjectileManager; // 用于获取所有子弹
class ZombieManager;     // 用于获取所有僵尸
class PlantManager;      // 用于获取所有植物
// class SunManager;     // 如果阳光也参与碰撞（例如，被僵尸阻挡，不太常见）
// class GamePlayState;  // 如果碰撞系统需要回调GamePlayState的特定方法

// 碰撞检测系统类
class CollisionSystem
{
public:
    CollisionSystem(); // 构造函数可以为空，或接收一些通用配置
    ~CollisionSystem() = default;

    // 主更新函数，由 GamePlayState 调用
    // 接收所有相关的实体管理器或实体列表的引用/指针
    void update(ProjectileManager &projectileManager,
                ZombieManager &zombieManager,
                PlantManager &plantManager
                /*, SunManager& sunManager (如果需要) */);

private:
    // 私有辅助方法，处理特定类型的碰撞
    void checkProjectileZombieCollisions(std::vector<Projectile *> &projectiles,
                                         std::vector<Zombie *> &zombies);

    void checkZombiePlantCollisions(std::vector<Zombie *> &zombies,
                                    std::vector<Plant *> &plants,
                                    float dt); // dt 可能用于攻击频率的判断（虽然攻击频率主要在Zombie内部管理）
                                               // 但如果碰撞系统要触发攻击动画的开始，可能会用到
                                               // 实际上，僵尸攻击植物的逻辑更多是在 Zombie::update 中处理索敌和状态转换

    // (未来可能需要的其他碰撞检查)
    // void checkPlayerSpecialAbilityCollisions(...);
};
