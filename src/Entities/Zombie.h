#pragma once

#include "Entity.h" // 继承自实体基类
#include <string>
#include <vector> // 确保包含 <vector>

// 前向声明
class ResourceManager;
class Plant; // 用于 update 和 findTargetPlant 的参数类型
class Grid;  // <--- 新增前向声明，如果 getLane 需要 Grid&

// 僵尸可能有的状态
enum class ZombieState
{
    WALKING,   // 行走
    ATTACKING, // 攻击
    DYING,     // 死亡中 (播放死亡动画)
    DEAD       // 已死亡 (等待移除)
};

// 僵尸基类，定义所有僵尸的共同行为和属性
class Zombie : public Entity
{
public:
    // 构造函数
    Zombie(ResourceManager &resManager, const std::string &textureKey,
           const sf::Vector2f &spawnPosition,
           int health, float speed, int damage,
           float attackInterval,
           Grid &grid); // <--- 新增 Grid 引用，用于准确计算行号

    // 虚析构函数由 Entity 继承而来
    ~Zombie() override = default;

    // 更新僵尸状态
    // dt: 帧间隔时间
    // plantsInLane: 当前僵尸所在行的植物列表
    virtual void update(float dt, const std::vector<Plant *> &plantsInLane);

    // 僵尸受到伤害
    virtual void takeDamage(int amount);

    // --- 状态查询 ---
    bool isAlive() const;
    bool isReadyToBeRemoved() const;
    ZombieState getCurrentState() const;

    // --- 状态管理 ---
    virtual void changeState(ZombieState newState);

    // --- 位置/逻辑相关 ---
    // 获取僵尸当前所在的行号 (现在使用内部的 m_gridRef)
    int getLane() const;

protected:
    int m_health;
    float m_speed;
    int m_damagePerAttack;
    float m_attackInterval;

    ZombieState m_currentState;
    float m_stateTimer;
    Plant *m_currentTargetPlant;
    Grid &m_gridRef; // <--- 新增: 对 Grid 的引用，用于 getLane

    // --- 行为辅助方法 ---
    virtual void moveLeft(float dt);
    // 在已筛选的、同一行的植物中寻找攻击目标
    Plant *findTargetPlant(const std::vector<Plant *> &plantsInLane);
    // 执行一次攻击动作
    virtual void attack(Plant *targetPlant);
};
