#include "Peashooter.h"
#include "../Core/ResourceManager.h"      // 包含资源管理器
#include "../Systems/Grid.h"              // 包含网格系统
#include "../Systems/ProjectileManager.h" // 包含子弹管理器
#include "../Systems/PlantManager.h"      // 如果构造函数需要 PlantManager
#include "../Utils/Constants.h"           // 包含常量定义
#include <iostream>                       // 用于调试输出
#include <cstdlib>                        // 为了 rand() (用于随机化初始射击计时)
// #include <ctime> (srand 已在 Game::Game() 中调用)

// Peashooter 构造函数实现
Peashooter::Peashooter(ResourceManager &resManager,
                       const sf::Vector2i &gridPos, // (行, 列)
                       Grid &gridSystem,
                       ProjectileManager &projectileManager
                       /* PlantManager* ownerManager */)
    : Plant(resManager,                          // 调用 Plant 基类构造函数
            PEASHOOTER_TEXTURE_KEY,              // 植物纹理的键名
            gridPos,                             // 网格位置
            gridSystem,                          // 网格系统引用
            PEASHOOTER_HEALTH,                   // 生命值 (来自常量)
            PEASHOOTER_COST),                    // 花费 (来自常量)
      m_projectileManagerRef(projectileManager), // 初始化子弹管理器引用
      m_shootTimer(0.0f),                        // 初始化射击计时器
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
        m_shootTimer -= m_shootInterval; // 重置计时器 (减去间隔比直接设为0更精确，以处理可能的帧延迟)

        // TODO: 在这里添加逻辑：只在当前行有僵尸时才射击
        // 这通常需要访问游戏状态或一个专门的僵尸管理器来查询信息。
        // 例如:
        // bool zombieInLane = m_ownerPlantManager->isZombieInLane(this->getRow(), this->getPosition().x);
        // if (zombieInLane) {
        //     shoot();
        // }
        // 当前阶段，我们先让它无条件射击，只要冷却完毕。
        shoot();
    }
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
    projectileStartPosition.y -= plantBounds.height * 0.05f; // 假设嘴部在精灵高度的5%偏上处 (如果原点在中心)
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
