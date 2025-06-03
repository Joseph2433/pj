#pragma once

#include "Entity.h" // 继承自实体基类
#include <string>

class ResourceManager;
class Plant; // 前向声明，用于攻击目标

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
    // textureKey: 在 ResourceManager 中的纹理键名
    // spawnPosition: 僵尸的初始生成位置
    // health: 初始生命值
    // speed: 移动速度 (通常为正值，方向由逻辑控制)
    // damage: 每次攻击造成的伤害
    Zombie(ResourceManager &resManager, const std::string &textureKey,
           const sf::Vector2f &spawnPosition,
           int health, float speed, int damage);

    ~Zombie() override = default; // 虚析构函数由 Entity 继承

    // 更新僵尸状态 (由派生类具体实现或部分实现)
    // dt: 帧间隔时间
    // plantsInLane: 当前行上的植物列表 (用于索敌和攻击，未来使用)
    virtual void update(float dt /*, const std::vector<Plant*>& plantsInLane */);

    // Entity 基类已有 draw()

    // 僵尸受到伤害
    // amount: 受到的伤害值
    virtual void takeDamage(int amount);

    // 检查僵尸是否还存活 (生命值 > 0 且不在死亡状态)
    bool isAlive() const;
    // 检查僵尸是否已彻底死亡并可被移除
    bool isReadyToBeRemoved() const;

    // --- 状态管理 ---
    ZombieState getCurrentState() const;
    virtual void changeState(ZombieState newState);

    // --- 攻击相关 (未来实现) ---
    // virtual void attack(Plant* targetPlant);
    // int getDamage() const;

    // 获取僵尸所在的行 (基于其Y坐标和网格信息)
    // gridCellHeight: 网格单元的高度
    // gridStartY: 网格区域的起始Y坐标
    int getLane(float gridCellHeight, float gridStartY) const;

protected:
    int m_health;          // 当前生命值
    float m_speed;         // 移动速度 (通常是向左移动的基础速度值)
    int m_damagePerAttack; // 每次攻击的伤害值

    ZombieState m_currentState; // 僵尸当前状态
    float m_stateTimer;         // 用于状态相关的计时 (例如攻击间隔、死亡动画时长)

    // 辅助方法，处理移动逻辑
    virtual void moveLeft(float dt);
};
