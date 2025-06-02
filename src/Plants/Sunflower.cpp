#include "Sunflower.h"
#include "../Core/ResourceManager.h" // 假设路径
#include "../Systems/Grid.h"         // 假设路径
#include "../Utils/Constants.h"      // 已经在 Sunflower.h 中包含了
// #include "../GameLogic/SunManager.h" // 如果需要直接调用 SunManager (未来可能)

#include <iostream> // 用于调试 (可选)

Sunflower::Sunflower(ResourceManager &resManager,
                     const sf::Vector2i &gridPos, // (row, column)
                     Grid &gridSystem)
    : Plant(resManager,
            "sunflower", // 这是在 ResourceManager 中注册的向日葵纹理的键名
                         // 你需要在加载资源时使用如:
                         // game->getResourceManager().loadTexture("sunflower", "assets/images/plants/sunflower.png");
            gridPos,
            gridSystem,
            SUNFLOWER_HEALTH, // 从 Constants.h 获取生命值
            SUNFLOWER_COST)   // 从 Constants.h 获取花费
// m_sunProductionTimer(0.0f),
// m_sunProductionInterval(Constants::SUNFLOWER_SUN_PRODUCTION_INTERVAL) // 如果定义了生产间隔常量
{
    // 向日葵特有的初始化代码
    // 例如，如果向日葵有多个动画帧（如发光动画），可以在这里设置动画
    // std::cout << "Sunflower created at grid (" << gridPos.x << "r, " << gridPos.y << "c)" << std::endl;

    // 设置一个合适的缩放，如果原始图片太大或太小
    // m_sprite.setScale(Constants::ENTITY_DEFAULT_SCALE * 0.9f, Constants::ENTITY_DEFAULT_SCALE * 0.9f); // 举例
    // centerOrigin(); // Plant的构造函数已经调用了centerOrigin
}

void Sunflower::update(float dt)
{
    // 首先调用基类 Plant 的 update 方法
    Plant::update(dt);

    // --- 向日葵特有的更新逻辑 ---
    // 在这个阶段，我们还没有实现阳光生产，所以这里可能为空。
    // 未来，这里会包含：
    // 1. 更新阳光生产计时器
    //    m_sunProductionTimer += dt;
    // 2. 检查是否可以生产阳光 (计时器到达间隔)
    //    if (m_sunProductionTimer >= m_sunProductionInterval) {
    //        produceSun();
    //        m_sunProductionTimer = 0.0f; // 重置计时器
    //    }
    // 3. 更新动画（如果向日葵有生产阳光的动画等）

    // 示例：简单地让它小幅度上下浮动（纯粹为了演示update可以做事）
    // static float elapsedTime = 0.0f;
    // elapsedTime += dt * 3.0f; // 加快速度
    // float offsetY = std::cos(elapsedTime) * 1.5f; // 上下浮动1.5像素
    // 这个简单的浮动示例也可能不理想，因为它会偏离格子中心
    // 更好的做法是动画不改变其通过 setPosition 设置的基准位置，而是改变其内部绘制偏移或使用更复杂的动画系统
    // 但 Plant 的构造函数已经将植物放置在格子中心，所以这种晃动是相对于格子中心的。
    // 对于一个简单的精灵，你可能需要记录原始的中心Y坐标，然后设置 m_sprite.setPosition(originalX, originalY + offsetY)
    // 或者，如果Entity支持动画帧，通过切换动画帧来实现。
}

// 未来实现的 produceSun 方法
// void Sunflower::produceSun() {
//    std::cout << "Sunflower at (" << getGridPosition().x << "r, " << getGridPosition().y << "c) produces sun!" << std::endl;
//    // 在这里创建并“掉落”一个阳光实体
//    // 需要一个 SunManager 来管理阳光的创建、收集等
//    // 例如: m_sunManagerRef->createSunAt(getPosition(), Constants::SUNFLOWER_SUN_AMOUNT);
// }
