#pragma once

#include "../Entities/Plant.h" // 继承自 Plant 基类
#include "../Utils/Constants.h"
// 前向声明
class ResourceManager;
class Grid;
// class SunManager; // 如果向日葵直接与阳光管理器交互（未来可能）

class Sunflower : public Plant
{
public:
    Sunflower(ResourceManager &resManager,
              const sf::Vector2i &gridPos, // 网格坐标 (行, 列)
              Grid &gridSystem);

    // 覆盖 Plant::update() 来实现向日葵特有的行为
    void update(float dt) override;

    // 未来可能添加的方法：
    // void produceSun(); // 生产阳光的方法

private:
    // 向日葵特有的属性可以放在这里
    // 例如，生产阳光的计时器
    // float m_sunProductionTimer;
    // float m_sunProductionInterval; // 可以从 Constants.h 初始化
};
