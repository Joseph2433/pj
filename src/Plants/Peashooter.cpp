#include "Peashooter.h"
#include "../Utils/Constants.h"
#include "../Core/ResourceManager.h"
#include "../Systems/Grid.h"

#include <iostream> // 用于调试 (可选)

Peashooter::Peashooter(ResourceManager &resManager,
                       const sf::Vector2i &gridPos, // (row, column)
                       Grid &gridSystem)
    : Plant(resManager,
            "peashooter", // 这是在 ResourceManager 中注册的豌豆射手纹理的键名
                          // 你需要在加载资源时使用如:
                          // game->getResourceManager().loadTexture("peashooter", "assets/images/plants/peashooter.png");
            gridPos,
            gridSystem,
            PEASHOOTER_HEALTH, // 从 Constants.h 获取生命值
            PEASHOOTER_COST)   // 从 Constants.h 获取花费
// m_shootTimer(0.0f),
// m_shootInterval(Constants::PEASHOOTER_SHOOT_INTERVAL) // 如果定义了射击间隔常量
{
    // 豌豆射手特有的初始化代码
    // 例如，如果豌豆射手有多个动画帧，可以在这里设置动画
    // std::cout << "Peashooter created at grid (" << gridPos.x << "r, " << gridPos.y << "c)" << std::endl;

    // 设置一个合适的缩放，如果原始图片太大或太小
    // m_sprite.setScale(Constants::ENTITY_DEFAULT_SCALE * 0.8f, Constants::ENTITY_DEFAULT_SCALE * 0.8f); // 举例
    // centerOrigin(); // Plant的构造函数已经调用了centerOrigin，这里通常不需要重复调用，除非你想基于新尺寸重新计算
}

void Peashooter::update(float dt)
{
    // 首先调用基类 Plant 的 update 方法
    Plant::update(dt);

    // --- 豌豆射手特有的更新逻辑 ---
    // 在这个阶段，我们还没有实现射击，所以这里可能为空。
    // 未来，这里会包含：
    // 1. 更新射击计时器
    //    m_shootTimer += dt;
    // 2. 检查是否可以射击 (计时器到达间隔，且前方有僵尸)
    //    if (m_shootTimer >= m_shootInterval) {
    //        // bool zombieInLane = checkForZombieInLane(); // 需要一个方法来检测同行的僵尸
    //        // if (zombieInLane) {
    //        //     shoot();
    //        //     m_shootTimer = 0.0f; // 重置计时器
    //        // }
    //    }
    // 3. 更新动画（如果豌豆射手有攻击动画等）

    // 示例：简单地让它左右晃动（纯粹为了演示update可以做事）
    // static float elapsedTime = 0.0f;
    // elapsedTime += dt;
    // float offsetX = std::sin(elapsedTime * 2.0f) * 2.0f; // 晃动幅度为2像素
    // sf::Vector2f currentPos = getPosition(); // 假设Plant基类没有直接提供原始放置位置
    // 我们需要知道它在网格单元中的原始中心点才能正确晃动
    // 这个简单的晃动示例可能不理想，因为它会偏离格子中心
    // 更好的做法是基于其在格子内的相对位置进行动画
}

// 未来实现的 shoot 方法
// void Peashooter::shoot() {
//    std::cout << "Peashooter at (" << getGridPosition().x << "r, " << getGridPosition().y << "c) shoots!" << std::endl;
//    // 在这里创建并发射一个豌豆子弹实体
//    // 需要一个 ProjectileManager 来管理子弹
// }
