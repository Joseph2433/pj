#pragma once

#include "Entity.h"
#include <SFML/System.hpp> // 用于 sf::Vector2i (网格位置)
#include <string>

// 前向声明
class ResourceManager;
class Grid;

class Plant : public Entity
{
public:
    // 构造函数:
    // health 和 cost 参数将由派生类 (如 Sunflower, Peashooter)
    // 在创建实例时，使用 Constants.h 中定义的特定植物常量来填充。
    Plant(ResourceManager &resManager,
          const std::string &textureKey,
          const sf::Vector2i &gridPos, // 预期为 (行, 列)
          Grid &gridSystem,
          int health, // 植物的生命值
          int cost);  // 植物的阳光花费

    virtual ~Plant() = default;

    void update(float dt) override;
    virtual void takeDamage(int damage);
    bool isAlive() const;

    int getCost() const;
    const sf::Vector2i &getGridPosition() const; // 返回 (行, 列)

    int getRow() const;
    int getColumn() const;

protected:
    int m_health;
    int m_cost;
    sf::Vector2i m_gridPosition; // 存储 (行, 列)
};
