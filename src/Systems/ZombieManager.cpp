#include "ZombieManager.h"
#include "../Entities/Zombie.h"     // 包含 Zombie.h (基类)
#include "../Zombies/BasicZombie.h" // 包含 BasicZombie.h (具体实现)
#include "../Core/ResourceManager.h"
#include "../Systems/Grid.h"    // 包含 Grid.h
#include "../Utils/Constants.h" // 包含常量
#include <SFML/Graphics/RenderWindow.hpp>
#include <algorithm> // For std::remove_if
#include <iostream>  // 用于调试

ZombieManager::ZombieManager(ResourceManager &resManager, Grid &grid)
    : m_resourceManagerRef(resManager), m_gridRef(grid)
{
}

ZombieManager::~ZombieManager() = default;

void ZombieManager::spawnZombie(int row, ZombieType type)
{
    // 1. 确定生成位置
    // Y 坐标：基于行和网格单元格高度，使僵尸大致在行的中间
    float spawnY = m_gridRef.getGridStartPosition().y + (static_cast<float>(row) + 0.5f) * m_gridRef.getCellSize().y;
    // X 坐标：在屏幕右侧之外一点
    float spawnX = static_cast<float>(WINDOW_WIDTH) + ZOMBIE_SPAWN_START_X_OFFSET;

    sf::Vector2f spawnPosition(spawnX, spawnY);

    // 2. 根据类型创建僵尸
    std::unique_ptr<Zombie> newZombie = nullptr;
    switch (type)
    {
    case ZombieType::BASIC:
        newZombie = std::make_unique<BasicZombie>(m_resourceManagerRef, spawnPosition);
        break;
    // case ZombieType::CONEHEAD:
    //     newZombie = std::make_unique<ConeheadZombie>(m_resourceManagerRef, spawnPosition);
    //     break;
    default:
        std::cerr << "ZombieManager: 请求生成未知类型的僵尸！" << std::endl;
        return;
    }

    if (newZombie)
    {
        m_zombies.push_back(std::move(newZombie));
        // std::cout << "ZombieManager: 在第 " << row << " 行生成了一个普通僵尸。" << std::endl;
    }
}

void ZombieManager::update(float dt, const sf::RenderWindow &window)
{
    // 1. 更新所有僵尸
    for (auto &zombie : m_zombies)
    {
        if (zombie && zombie->isAlive())
        { // 只更新存活的僵尸
            // 未来，这里可能需要传递植物列表给僵尸的 update 用于索敌
            // std::vector<Plant*> plantsInLane = m_plantManagerRef.getPlantsInRow(zombie->getLane(...));
            zombie->update(dt /*, plantsInLane */);

            // 2. 检查僵尸是否到达房子 (游戏失败条件之一)
            // 注意：这个逻辑也可以放在 GamePlayState 中处理
            if (zombie->getPosition().x < ZOMBIE_REACHED_HOUSE_X)
            {
                std::cout << "游戏失败：一个僵尸到达了你的房子！" << std::endl;
                // TODO: 触发游戏结束状态
                // m_gameStateRef.triggerGameOver();
                zombie->changeState(ZombieState::DEAD); // 简单处理，让它被移除
            }
        }
    }

    // 3. 移除已死亡 (isReadyToBeRemoved() 返回 true) 的僵尸
    m_zombies.erase(
        std::remove_if(m_zombies.begin(), m_zombies.end(),
                       [](const std::unique_ptr<Zombie> &z_ptr)
                       {
                           if (!z_ptr)
                               return true; // 移除空指针
                           return z_ptr->isReadyToBeRemoved();
                       }),
        m_zombies.end());
}

void ZombieManager::draw(sf::RenderWindow &window)
{
    for (const auto &zombie : m_zombies)
    {
        if (zombie)
        { // 确保指针有效再绘制
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

// const std::vector<std::unique_ptr<Zombie>>& ZombieManager::getAllZombies() const {
//     return m_zombies;
// }
