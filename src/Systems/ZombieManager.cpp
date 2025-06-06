#include "ZombieManager.h"
#include "../Entities/Zombie.h"
#include "../Zombies/BasicZombie.h"
#include "../Zombies/BigZombie.h"
#include "../Zombies/BossZombie.h"
#include "../Zombies/QuickZombie.h"
#include "../Core/ResourceManager.h"
#include "../Systems/Grid.h"
#include "../Utils/Constants.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <algorithm>
#include <iostream>

ZombieManager::ZombieManager(ResourceManager &resManager, Grid &grid)
    : m_resourceManagerRef(resManager), m_gridRef(grid)
{
}

ZombieManager::~ZombieManager() = default;

void ZombieManager::spawnZombie(int row, ZombieType type)
{
    // 生成位置
    // Y ,大致在行的中间
    float spawnY = m_gridRef.getGridStartPosition().y + (static_cast<float>(row) + 0.5f) * m_gridRef.getCellSize().y + 15;
    // X ,在屏幕右侧之外一点
    float spawnX = static_cast<float>(WINDOW_WIDTH) + ZOMBIE_SPAWN_START_X_OFFSET;

    sf::Vector2f spawnPosition(spawnX, spawnY);

    // 创建僵尸
    std::unique_ptr<Zombie> newZombie = nullptr;
    switch (type)
    {
    case ZombieType::BASIC:
        newZombie = std::make_unique<BasicZombie>(m_resourceManagerRef, spawnPosition, m_gridRef);
        break;
    case ZombieType::BIG:
        newZombie = std::make_unique<BigZombie>(m_resourceManagerRef, spawnPosition, m_gridRef);
        break;
    case ZombieType::BOSS:
        newZombie = std::make_unique<BossZombie>(m_resourceManagerRef, spawnPosition, m_gridRef);
        break;
    case ZombieType::QUICK:
        newZombie = std::make_unique<QuickZombie>(m_resourceManagerRef, spawnPosition, m_gridRef);
        break;
    default:
        std::cerr << "ZombieManager: undefined type zombie!" << std::endl;
        return;
    }

    if (newZombie)
    {
        m_zombies.push_back(std::move(newZombie));
        std::cout << "ZombieManager: Spawned a zombie of type " << static_cast<int>(type)
                  << " in row " << row << std::endl;
    }
}
void ZombieManager::update(float dt, const sf::RenderWindow &window)
{
    m_zombies.erase(
        std::remove_if(m_zombies.begin(), m_zombies.end(),
                       [](const std::unique_ptr<Zombie> &z_ptr)
                       {
                           if (!z_ptr)
                               return true;
                           return z_ptr->isReadyToBeRemoved();
                       }),
        m_zombies.end());
}

void ZombieManager::draw(sf::RenderWindow &window)
{
    for (const auto &zombie : m_zombies)
    {
        if (zombie)
        {
            zombie->draw(window);
        }
    }
}

void ZombieManager::clear()
{
    m_zombies.clear();
}

std::vector<Zombie *> ZombieManager::getActiveZombies()
{
    std::vector<Zombie *> activeZombies;
    for (const auto &zombie_ptr : m_zombies)
    {
        if (zombie_ptr && zombie_ptr->isAlive())
        { // 只返回逻辑上存活的僵尸
            activeZombies.push_back(zombie_ptr.get());
        }
    }
    return activeZombies;
}
