#include "IcePeashooter.h"
#include "../Core/ResourceManager.h"
#include "../Systems/Grid.h"
#include "../Systems/ProjectileManager.h"
#include "../Systems/PlantManager.h"
#include "../Projectiles/IcePea.h"
#include "../Utils/Constants.h"
#include "../Entities/Zombie.h"
#include <iostream>
#include <SFML/System/Vector2.hpp>

IcePeashooter::IcePeashooter(ResourceManager &resManager, const sf::Vector2i &gridPos, Grid &gridSystem,
                             PlantManager &plantManager, ProjectileManager &projectileManager)
    : Plant(resManager,
            ICE_PEASHOOTER_TEXTURE_KEY,
            gridPos,
            gridSystem,
            ICE_PEASHOOTER_HEALTH,
            ICE_PEASHOOTER_COST),
      m_projectileManagerRef(projectileManager),
      m_plantManagerRef(plantManager),
      m_localResManagerRef(resManager),
      m_shootInterval(ICE_PEASHOOTER_SHOOT_INTERVAL)
{
    m_shootTimerClock.restart();
    std::cout << "IcePeashooter created." << std::endl;
}

void IcePeashooter::update(float dt)
{
    Plant::update(dt);

    if (m_shootTimerClock.getElapsedTime().asSeconds() >= m_shootInterval)
    {
        if (checkForZombiesInLane())
        {
            shoot();
            m_shootTimerClock.restart(); // 重置计时器
        }
    }
}

bool IcePeashooter::checkForZombiesInLane() const
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

void IcePeashooter::shoot()
{
    sf::Vector2f shootPosition = getPosition();
    sf::FloatRect plantBounds = getGlobalBounds();
    shootPosition.x += plantBounds.width * 0.35f;
    shootPosition.y -= plantBounds.height * 0.10f;
    sf::Vector2f shootDirection(1.0f, 0.0f);

    m_projectileManagerRef.addProjectile(
        std::make_unique<IcePea>(m_localResManagerRef, shootPosition, shootDirection));
    std::cout << "IcePeashooter at (" << getGridPosition().x << "," << getGridPosition().y
              << ") fired an IcePea." << std::endl;
}
