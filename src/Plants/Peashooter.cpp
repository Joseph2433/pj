#include "Peashooter.h"
#include "../Core/ResourceManager.h"
#include "../Systems/Grid.h"
#include "../Systems/ProjectileManager.h"
#include "../Systems/PlantManager.h"
#include "../Utils/Constants.h"
#include "../Entities/Zombie.h"
#include "../Projectiles/Pea.h"
#include <iostream>
#include <cstdlib>

Peashooter::Peashooter(ResourceManager &resManager,
                       const sf::Vector2i &gridPos,
                       Grid &gridSystem,
                       PlantManager &plantManager,
                       ProjectileManager &projectileManager)
    : Plant(resManager,
            PEASHOOTER_TEXTURE_KEY,
            gridPos,
            gridSystem,
            PEASHOOTER_HEALTH,
            PEASHOOTER_COST),
      m_projectileManagerRef(projectileManager),
      m_plantManagerRef(plantManager),
      m_localResManagerRef(resManager),
      m_shootTimer(0.0f),
      m_shootInterval(PEASHOOTER_SHOOT_INTERVAL)
{
    if (m_shootInterval > 0.001f)
    {
        m_shootTimer = static_cast<float>(rand() % static_cast<int>(m_shootInterval * 1000.0f)) / 1000.0f;
    }
}

void Peashooter::update(float dt)
{
    Plant::update(dt);

    m_shootTimer += dt;

    if (m_shootTimer >= m_shootInterval)
    {
        m_shootTimer -= m_shootInterval;
        if (checkForZombiesInLane())
        {
            shoot();
            m_shootTimer = 0.0f;
        }
    }
}

bool Peashooter::checkForZombiesInLane() const
{
    std::vector<Zombie *> zombiesInLane = m_plantManagerRef.getZombiesInLane(this->getRow());
    for (Zombie *zombie : zombiesInLane)
    {
        if (zombie && zombie->isAlive() && zombie->getPosition().x > this->getPosition().x)
        {
            return true;
        }
    }
    return false;
}

void Peashooter::shoot()
{
    sf::Vector2f projectileStartPosition = getPosition();
    sf::FloatRect plantBounds = getGlobalBounds();
    projectileStartPosition.x += plantBounds.width * 0.35f;
    projectileStartPosition.y -= plantBounds.height * 0.55f;

    // 创建 Pea
    auto pea = std::make_unique<Pea>(m_localResManagerRef, projectileStartPosition, sf::Vector2f(1.f, 0.f));

    // 发射
    m_projectileManagerRef.addProjectile(std::move(pea));

    std::cout << "Peashooter at (" << getGridPosition().x << "," << getGridPosition().y << ") fired a Pea." << std::endl;
}