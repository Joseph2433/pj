#include "Sunflower.h"
#include "../Systems/PlantManager.h" // 包含 PlantManager.h
#include "../Utils/Constants.h"      // 为了 SUNFLOWER_SUN_PRODUCTION_INTERVAL
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For std::time for srand, (srand已在Game.cpp中全局调用)

Sunflower::Sunflower(ResourceManager &resManager, const sf::Vector2i &gridPos, Grid &gridSystem, PlantManager &plantManager)
    : Plant(resManager, SUNFLOWER_TEXTURE_KEY, gridPos, gridSystem,
            SUNFLOWER_HEALTH, SUNFLOWER_COST),
      m_produceSunTimer(0.0f),
      m_plantManagerRef(plantManager)
{
    // 给一个随机的初始计时器值，避免所有向日葵同时产生阳光
    if (SUNFLOWER_SUN_PRODUCTION_INTERVAL > 0.001f)
    { // 避免除以0
        m_produceSunTimer = static_cast<float>(rand() % static_cast<int>(SUNFLOWER_SUN_PRODUCTION_INTERVAL * 1000.f)) / 1000.f;
    }
    // std::cout << "向日葵创建，初始阳光生产计时器: " << m_produceSunTimer << std::endl;
}

void Sunflower::update(float dt)
{
    Plant::update(dt); // 调用基类更新

    m_produceSunTimer += dt;
    if (m_produceSunTimer >= SUNFLOWER_SUN_PRODUCTION_INTERVAL)
    {
        m_produceSunTimer -= SUNFLOWER_SUN_PRODUCTION_INTERVAL; // 重置计时器

        // std::cout << "向日葵 (" << getGridPosition().x << "r," << getGridPosition().y << "c) 请求产生阳光..." << std::endl;
        m_plantManagerRef.requestSunSpawnFromPlant(this); // 通过 PlantManager 请求产生阳光
    }
}
