#include "Peashooter.h"
#include "../Core/ResourceManager.h"      // 包含资源管理器
#include "../Systems/Grid.h"              // 包含网格系统
#include "../Systems/ProjectileManager.h" // 包含子弹管理器
#include "../Systems/PlantManager.h"
#include "../Utils/Constants.h" // 包含常量定义
#include "../Entities/Zombie.h" // 为了在 checkForZombiesInLane 中使用 Zombie 类型
#include <iostream>             // 用于调试输出
#include <cstdlib>              // 为了 rand() (用于随机化初始射击计时)
// #include <ctime> (srand 已在 Game::Game() 中调用)

// Peashooter 构造函数实现
Peashooter::Peashooter(ResourceManager &resManager,
                       const sf::Vector2i &gridPos, // (行, 列)
                       Grid &gridSystem,
                       PlantManager &plantManager,
                       ProjectileManager &projectileManager
                       /* PlantManager* ownerManager */)
    : Plant(resManager,                          // 调用 Plant 基类构造函数
            PEASHOOTER_TEXTURE_KEY,              // 植物纹理的键名
            gridPos,                             // 网格位置
            gridSystem,                          // 网格系统引用
            PEASHOOTER_HEALTH,                   // 生命值 (来自常量)
            PEASHOOTER_COST),                    // 花费 (来自常量)
      m_projectileManagerRef(projectileManager), // 初始化子弹管理器引用
      m_plantManagerRef(plantManager),
      m_shootTimer(0.0f), // 初始化射击计时器
      m_shootInterval(PEASHOOTER_SHOOT_INTERVAL)
{ // 从常量获取射击间隔

    // 为了避免所有同时种植的豌豆射手在完全相同的时间点首次射击，
    // 可以给 m_shootTimer 一个小的随机初始值 (0 到 m_shootInterval 之间)。
    if (m_shootInterval > 0.001f)
    { // 避免除以0或过小的值
        m_shootTimer = static_cast<float>(rand() % static_cast<int>(m_shootInterval * 1000.0f)) / 1000.0f;
    }

    // 可选: 设置子弹发射点的精确偏移（如果需要）
    // 这个偏移量是相对于 getPosition() 返回的点（通常是精灵中心）
    // 你需要根据你的豌豆射手精灵图来调整这些值
    // 例如，如果精灵宽度为 W, 高度为 H, 原点在中心:
    // m_shootOriginOffset = sf::Vector2f(W * 0.3f, -H * 0.1f); // 假设嘴巴在右前方偏上一点
    // 或者在 shoot() 方法中动态计算

    // std::cout << "豌豆射手创建于网格 (" << gridPos.x << "行, " << gridPos.y << "列)，初始射击计时: " << m_shootTimer << std::endl;
}

// Peashooter 每帧的更新逻辑
void Peashooter::update(float dt)
{
    Plant::update(dt); // 首先调用基类 Plant 的 update 方法 (如果它有逻辑的话)

    // 更新射击计时器
    m_shootTimer += dt;

    // 检查是否到达射击时间
    if (m_shootTimer >= m_shootInterval)
    {
        m_shootTimer -= m_shootInterval;
        if (checkForZombiesInLane())
        { // 只有前方有僵尸才射击
            shoot();
            m_shootTimer = 0.0f;
        }
        // 如果没有僵尸，计时器可以不重置，或者部分重置以实现更快的首次射击
        // 当前设计：如果没有僵尸，计时器会继续累积，直到下一次检查到僵尸且时间也到了才射击
        // 如果想没僵尸时也重置，可以把 m_shootTimer -= m_shootInterval; 移到 if 外部
        // 但通常是只有射击了才消耗冷却时间
    }
}

// 检查当前行是否有僵尸
bool Peashooter::checkForZombiesInLane() const
{
    // 通过 PlantManager 获取当前行所有僵尸的原始指针
    // getRow() 返回植物的行号
    // getPosition().x 返回植物的X坐标，我们只关心植物右方的僵尸
    std::vector<Zombie *> zombiesInLane = m_plantManagerRef.getZombiesInLane(this->getRow());
    for (Zombie *zombie : zombiesInLane)
    {
        if (zombie && zombie->isAlive() && zombie->getPosition().x > this->getPosition().x)
        {
            // 如果僵尸存活且在豌豆射手的右边（即前方）
            // std::cout << "Peashooter sees zombie in lane!" << std::endl;
            return true; // 发现僵尸，可以射击
        }
    }
    return false; // 没有发现前方僵尸
}

// 私有辅助方法：执行射击动作
void Peashooter::shoot()
{
    // 1. 计算子弹的起始位置
    // getPosition() 返回的是植物精灵的原点位置 (通常在 Plant 基类中通过 centerOrigin() 设置为其中心)
    sf::Vector2f projectileStartPosition = getPosition();

    // 2. 根据豌豆射手的视觉效果调整子弹的精确发射点 (例如，“嘴部”)
    // 这些偏移值需要你根据你的精灵图资源进行微调。
    // getGlobalBounds() 可以获取精灵在世界坐标中的边界框。
    sf::FloatRect plantBounds = getGlobalBounds();
    projectileStartPosition.x += plantBounds.width * 0.35f;  // 假设嘴部在精灵宽度的35%偏右处 (如果原点在中心)
                                                             // 如果原点在左边，可能是 plantBounds.width * 0.8f
    projectileStartPosition.y -= plantBounds.height * 0.85f; // 假设嘴部在精灵高度的5%偏上处 (如果原点在中心)
                                                             // 或者 projectileStartPosition.y += Y_OFFSET_FROM_CENTER_TO_MOUTH;

    // (可选) 如果在构造函数中设置了 m_shootOriginOffset:
    // projectileStartPosition += m_shootOriginOffset;

    // 3. 发射子弹
    // 豌豆的默认飞行方向是向右 (1, 0)
    // 速度和伤害从 Constants.h 获取
    m_projectileManagerRef.firePea(projectileStartPosition, sf::Vector2f(1.f, 0.f));
    // std::cout << "豌豆射手 (" << getGridPosition().x << "行," << getGridPosition().y << "列) 发射了一颗豌豆！" << std::endl;

    // 可选：播放射击音效
    // m_soundManagerRef.playSound(SoundID::PEASHOOTER_SHOOT);

    // 可选：触发射击动画 (如果豌豆射手有射击动画)
    // startShootingAnimation();
}
