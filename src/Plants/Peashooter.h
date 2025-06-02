#pragma once

#include "../Entities/Plant.h" // 继承自 Plant 基类
#include "../Utils/Constants.h"

// 前向声明
class ResourceManager;
class Grid;

class Peashooter : public Plant
{
public:
    Peashooter(ResourceManager &resManager,
               const sf::Vector2i &gridPos, // 网格坐标 (行, 列)
               Grid &gridSystem);

    // 覆盖 Plant::update() 来实现豌豆射手特有的行为
    void update(float dt) override;

    // 未来可能添加的方法：
    // void shoot(); // 发射子弹的方法

private:
    // 豌豆射手特有的属性可以放在这里
    // 例如，射击计时器，用于控制射击频率
    // float m_shootTimer;
    // float m_shootInterval; // 可以从 Constants.h 初始化
};
