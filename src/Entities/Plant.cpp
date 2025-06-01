#include "Plant.h"

// 假设的网格配置常量
// 你需要根据实际游戏设计调整这些值
const float GRID_WIDTH = 80.0f;    // 每个网格的宽度
const float GRID_HEIGHT = 100.0f;  // 每个网格的高度
const float GRID_START_X = 220.0f; // 网格起始X坐标
const float GRID_START_Y = 120.0f; // 网格起始Y坐标

Plant::Plant(int gridX, int gridY)
    : Entity() // 调用基类构造函数
      ,
      m_gridX(gridX), m_gridY(gridY), m_cost(0), m_cooldownTime(1.0f) // 默认1秒冷却
      ,
      m_actionTimer(m_cooldownTime)
{
    // 设置植物在屏幕上的位置
    sf::Vector2f screenPos = gridToScreen(gridX, gridY);
    setPosition(screenPos);

    // 重置行动计时器（开始计时）
    m_actionTimer.reset();
}

void Plant::update(float deltaTime)
{
    // 调用基类的update方法
    Entity::update(deltaTime);

    // 更新行动计时器
    m_actionTimer.update(deltaTime);

    // 如果可以执行行动，则执行
    if (canPerformAction())
    {
        performAction();
        // 重置计时器以开始新的冷却周期
        m_actionTimer.reset();
    }
}

void Plant::render(sf::RenderWindow &window)
{
    // 调用基类的render方法
    Entity::render(window);

    // 植物特有的渲染逻辑可以在这里添加
    // 比如绘制冷却指示器、特效等
}

sf::Vector2f Plant::gridToScreen(int gridX, int gridY) const
{
    // 将网格坐标转换为屏幕像素坐标
    float screenX = GRID_START_X + gridX * GRID_WIDTH;
    float screenY = GRID_START_Y + gridY * GRID_HEIGHT;

    return sf::Vector2f(screenX, screenY);
}