#include "Peashooter.h"
#include "../Core/ResourceManager.h"
#include "../Systems/Grid.h"
#include "../Systems/ProjectileManager.h"
#include "../Systems/PlantManager.h"
#include "../Utils/Constants.h"
#include "../Entities/Zombie.h"
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
    // 1. 计算子弹的起始位置
    sf::Vector2f projectileStartPosition = getPosition();

    // 2. 调整子弹的精确发射点 (例如，“嘴部”)
    sf::FloatRect plantBounds = getGlobalBounds();
    projectileStartPosition.x += plantBounds.width * 0.35f;
    projectileStartPosition.y -= plantBounds.height * 0.55f;

    // 3. 发射子弹
    m_projectileManagerRef.firePea(projectileStartPosition, sf::Vector2f(1.f, 0.f));

    // 可选：播放射击音效
    // m_soundManagerRef.playSound(SoundID::PEASHOOTER_SHOOT);

    // 可选：触发射击动画 (如果豌豆射手有射击动画)
    // startShootingAnimation();
}
