#pragma once

#include "../Entities/Plant.h"
#include "../Utils/Constants.h"

class ResourceManager;
class Grid;
class PlantManager;
class ProjectileManager;

class Peashooter : public Plant
{
public:
    Peashooter(ResourceManager &resManager,
               const sf::Vector2i &gridPos,
               Grid &gridSystem,
               PlantManager &plantManager,
               ProjectileManager &projectileManager);
    ~Peashooter() override = default;
    void update(float dt) override;

private:
    void shoot();
    bool checkForZombiesInLane() const;
    ProjectileManager &m_projectileManagerRef;
    PlantManager &m_plantManagerRef;
    ResourceManager &m_localResManagerRef;
    float m_shootTimer;
    const float m_shootInterval;
};
