#pragma once
#include "../Entities/Plant.h"

class ResourceManager;
class Grid;
class PlantManager;
class ProjectileManager;

class IcePeashooter : public Plant
{
public:
    IcePeashooter(ResourceManager &resManager,
                  const sf::Vector2i &gridPos,
                  Grid &gridSystem,
                  PlantManager &plantManager,
                  ProjectileManager &projectileManager);
    ~IcePeashooter() override = default;

    void update(float dt) override;

private:
    void shoot();
    bool checkForZombiesInLane() const;

    ProjectileManager &m_projectileManagerRef;
    PlantManager &m_plantManagerRef;
    ResourceManager &m_localResManagerRef;
    sf::Clock m_shootTimerClock;
    float m_shootInterval;
};
