#include "Sunflower.h"
#include "../Systems/PlantManager.h"
#include "../Utils/Constants.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Sunflower::Sunflower(ResourceManager &resManager, const sf::Vector2i &gridPos, Grid &gridSystem, PlantManager &plantManager)
    : Plant(resManager, SUNFLOWER_TEXTURE_KEY, gridPos, gridSystem,
            SUNFLOWER_HEALTH, SUNFLOWER_COST),
      m_produceSunTimer(0.0f),
      m_plantManagerRef(plantManager)
{
    // 给一个随机的初始计时器值，避免所有向日葵同时产生阳光
    if (SUNFLOWER_SUN_PRODUCTION_INTERVAL > 0.001f)
    {
        m_produceSunTimer = static_cast<float>(rand() % static_cast<int>(SUNFLOWER_SUN_PRODUCTION_INTERVAL * 1000.f)) / 1000.f;
    }
}

void Sunflower::update(float dt)
{
    Plant::update(dt);

    m_produceSunTimer += dt;
    if (m_produceSunTimer >= SUNFLOWER_SUN_PRODUCTION_INTERVAL)
    {
        // 重置计时器
        m_produceSunTimer -= SUNFLOWER_SUN_PRODUCTION_INTERVAL;

        // 请求产生阳光
        m_plantManagerRef.requestSunSpawnFromPlant(this);
    }
}
