#include "Plant.h"
#include "../Core/ResourceManager.h" // 调整路径（如果需要）
#include "../Systems/Grid.h"         // 调整路径（如果需要）
// #include "../Utils/Constants.h" // Plant.cpp 本身不直接使用 Constants.h 中的特定植物属性
// 但派生类会使用 Constants.h 并将值传递给此构造函数

#include <iostream> // 用于调试 (可选)

Plant::Plant(ResourceManager &resManager,
             const std::string &textureKey,
             const sf::Vector2i &gridPos, // gridPos.x 是行, gridPos.y 是列
             Grid &gridSystem,
             int health, // 从派生类接收的具体植物生命值
             int cost)   // 从派生类接收的具体植物花费
    : Entity(resManager.getTexture(textureKey)),
      m_health(health),
      m_cost(cost),
      m_gridPosition(gridPos)
{

    sf::Vector2f cellCenterPixelPosition = gridSystem.getWorldPosition(gridPos.x, gridPos.y);
    centerOrigin(); // Entity 基类的方法
    setPosition(cellCenterPixelPosition);

    // 例如: std::cout << "Plant with health " << m_health << " and cost " << m_cost << " created." << std::endl;
}

void Plant::update(float dt)
{
    Entity::update(dt);
    // 植物基类通用的更新逻辑
}

void Plant::takeDamage(int damage)
{
    m_health -= damage;
    if (m_health < 0)
    {
        m_health = 0;
    }
}

bool Plant::isAlive() const
{
    return m_health > 0;
}

int Plant::getCost() const
{
    return m_cost;
}

const sf::Vector2i &Plant::getGridPosition() const
{
    return m_gridPosition;
}

int Plant::getRow() const
{
    return m_gridPosition.x;
}

int Plant::getColumn() const
{
    return m_gridPosition.y;
}
