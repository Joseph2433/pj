#include "Plant.h"
#include "../Core/ResourceManager.h"
#include "../Systems/Grid.h"
#include <iostream>

Plant::Plant(ResourceManager &resManager,
             const std::string &textureKey,
             const sf::Vector2i &gridPos,
             Grid &gridSystem,
             int health,
             int cost)
    : Entity(resManager.getTexture(textureKey)),
      m_health(health),
      m_cost(cost),
      m_gridPosition(gridPos)
{

    sf::Vector2f cellCenterPixelPosition = gridSystem.getWorldPosition(gridPos.x, gridPos.y);
    centerOrigin();
    setPosition(cellCenterPixelPosition);
}

void Plant::update(float dt)
{
    Entity::update(dt);
}

void Plant::takeDamage(int damage)
{
    if (!isAlive())
        return;

    m_health -= damage;
    if (m_health <= 0)
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
