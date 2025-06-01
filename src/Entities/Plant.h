#ifndef PLANT_H
#define PLANT_H

#include "Entity.h"
#include "../Utils/Timer.h"

// Plant继承自Entity，是所有植物的基类
// 它添加了植物特有的属性，如网格位置、冷却时间等
class Plant : public Entity
{
public:
    Plant(int gridX, int gridY);
    virtual ~Plant() = default;

    // 重写基类方法
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

    // 植物特有的方法
    virtual void performAction() {} // 植物的特殊行为（射击、产生阳光等）

    // 网格位置相关
    int getGridX() const { return m_gridX; }
    int getGridY() const { return m_gridY; }

    // 冷却时间管理
    void setCooldown(float cooldown) { m_cooldownTime = cooldown; }
    bool canPerformAction() const { return m_actionTimer.isExpired(); }

    // 植物成本
    void setCost(int cost) { m_cost = cost; }
    int getCost() const { return m_cost; }

protected:
    int m_gridX, m_gridY; // 在网格中的位置
    int m_cost;           // 种植成本（阳光）
    float m_cooldownTime; // 行动冷却时间
    Timer m_actionTimer;  // 行动计时器

    // 将网格坐标转换为屏幕坐标
    sf::Vector2f gridToScreen(int gridX, int gridY) const;
};

#endif