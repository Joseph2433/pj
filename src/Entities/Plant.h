#pragma once

#include "Entity.h"
#include <SFML/System.hpp>
#include <string>

class ResourceManager;
class Grid;

class Plant : public Entity
{
public:
    Plant(ResourceManager &resManager,
          const std::string &textureKey,
          const sf::Vector2i &gridPos,
          Grid &gridSystem,
          int health,
          int cost);
    virtual ~Plant() = default;
    void update(float dt) override;
    virtual void takeDamage(int damage);
    bool isAlive() const;
    int getCost() const;
    const sf::Vector2i &getGridPosition() const;
    int getRow() const;
    int getColumn() const;
    int getHealth() const { return m_health; };

protected:
    int m_health;
    int m_cost;
    sf::Vector2i m_gridPosition;
};
