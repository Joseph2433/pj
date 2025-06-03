#include "PlantManager.h"
#include "../Entities/Plant.h"    // 需要 Plant.h 的完整定义 (因为 unique_ptr析构)
#include "../Plants/Sunflower.h"  // 向日葵实现
#include "../Plants/Peashooter.h" // 豌豆射手实现
#include "../Core/ResourceManager.h"
#include "../Systems/Grid.h"
#include "../States/GamePlayState.h"      // GamePlayState定义
#include "../Systems/ProjectileManager.h" // ProjectileManager定义 (虽然这里不直接用，但Peashooter需要)
#include <SFML/Graphics/RenderWindow.hpp>
#include <algorithm>
#include <iostream>

// 修改构造函数以接收并存储 ProjectileManager 引用
PlantManager::PlantManager(ResourceManager &resManager, Grid &gridSystem,
                           GamePlayState &gameState, ProjectileManager &projectileManager)
    : m_resourceManagerRef(resManager),
      m_gridRef(gridSystem),
      m_gameStateRef(gameState),
      m_projectileManagerRef_forPlants(projectileManager)
{
}

PlantManager::~PlantManager() = default;

// 私有创建方法实现
std::unique_ptr<Plant> PlantManager::createSunflower(const sf::Vector2i &gridPosition)
{
    // 向日葵构造函数需要 PlantManager 引用 (this) 用于产阳光回调
    return std::make_unique<Sunflower>(m_resourceManagerRef, gridPosition, m_gridRef, *this);
}

std::unique_ptr<Plant> PlantManager::createPeashooter(const sf::Vector2i &gridPosition)
{
    // 豌豆射手构造函数需要 ProjectileManager 引用
    return std::make_unique<Peashooter>(m_resourceManagerRef, gridPosition, m_gridRef, m_projectileManagerRef_forPlants);
}

bool PlantManager::tryAddPlant(PlantType type, const sf::Vector2i &gridPosition)
{
    if (!m_gridRef.isValidGridPosition(gridPosition) || isCellOccupied(gridPosition))
    {
        return false;
    }

    std::unique_ptr<Plant> newPlant = nullptr;
    switch (type)
    {
    case PlantType::SUNFLOWER:
        newPlant = createSunflower(gridPosition); // 调用私有创建方法
        break;
    case PlantType::PEASHOOTER:
        newPlant = createPeashooter(gridPosition); // 调用私有创建方法
        break;
    // ... 其他植物类型，每种都对应一个 createXYZ 方法 ...
    default:
        std::cerr << "PlantManager: 未知植物类型！ 类型ID: " << static_cast<int>(type) << std::endl;
        return false;
    }

    if (newPlant)
    {
        // (未来在这里加入阳光花费检查，例如：)
        // if (m_gameStateRef.getSunManager().trySpendSun(newPlant->getCost())) {
        //     m_plants.push_back(std::move(newPlant));
        //     return true;
        // } else {
        //     std::cout << "阳光不足以种植此植物！" << std::endl;
        //     return false;
        // }
        m_plants.push_back(std::move(newPlant)); // 当前不检查阳光
        std::cout << "PlantManager: 已种植植物类型 " << static_cast<int>(type) << " 于网格 (" << gridPosition.x << "行, " << gridPosition.y << "列)" << std::endl;

        return true;
    }
    return false;
}

void PlantManager::update(float dt)
{
    for (auto &plant : m_plants)
    {
        plant->update(dt);
    }
    m_plants.erase(
        std::remove_if(m_plants.begin(), m_plants.end(),
                       [](const std::unique_ptr<Plant> &p)
                       { return !p->isAlive(); }),
        m_plants.end());
}

void PlantManager::draw(sf::RenderWindow &window)
{
    for (const auto &plant : m_plants)
    {
        plant->draw(window);
    }
}

void PlantManager::clear()
{
    m_plants.clear();
}

bool PlantManager::isCellOccupied(const sf::Vector2i &gridPosition) const
{
    for (const auto &plant : m_plants)
    {
        if (plant->getGridPosition() == gridPosition)
        {
            return true;
        }
    }
    return false;
}

const std::vector<std::unique_ptr<Plant>> &PlantManager::getAllPlants() const
{
    return m_plants;
}

std::vector<Plant *> PlantManager::getPlantsInRow(int gridRow)
{
    std::vector<Plant *> plantsInRow;
    for (const auto &plantPtr : m_plants)
    {
        if (plantPtr->isAlive() && plantPtr->getRow() == gridRow)
        {
            plantsInRow.push_back(plantPtr.get());
        }
    }
    return plantsInRow;
}

// 当植物（如向日葵）调用此方法时，PlantManager 会请求 GamePlayState 产生阳光
void PlantManager::requestSunSpawnFromPlant(Plant *requestingPlant)
{
    if (requestingPlant)
    {
        m_gameStateRef.spawnSunFromPlant(requestingPlant); // 调用 GamePlayState 的方法
    }
}