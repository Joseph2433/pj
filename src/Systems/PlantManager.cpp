#include "PlantManager.h"
#include "../Entities/Plant.h" // 需要 Plant.h 的完整定义，因为 unique_ptr 的析构
#include "../Plants/Sunflower.h"
#include "../Plants/Peashooter.h"
#include "../Core/ResourceManager.h"
#include "../Systems/Grid.h"
#include "../States/GamePlayState.h" // 包含 GamePlayState.h
#include <SFML/Graphics/RenderWindow.hpp>
#include <algorithm>
#include <iostream>

PlantManager::PlantManager(ResourceManager &resManager, Grid &gridSystem, GamePlayState &gameState)
    : m_resourceManagerRef(resManager),
      m_gridRef(gridSystem),
      m_gameStateRef(gameState) {}

PlantManager::~PlantManager() = default; // 在cpp中default，确保Plant类型完整

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
        newPlant = std::make_unique<Sunflower>(m_resourceManagerRef, gridPosition, m_gridRef, *this); // 传递 *this (PlantManager)
        break;
    case PlantType::PEASHOOTER:
        newPlant = std::make_unique<Peashooter>(m_resourceManagerRef, gridPosition, m_gridRef); // Peashooter 构造函数可能不同
        break;
    default:
        std::cerr << "PlantManager: 未知植物类型！" << std::endl;
        return false;
    }
    if (newPlant)
    {
        m_plants.push_back(std::move(newPlant));
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

void PlantManager::clear() { m_plants.clear(); }

bool PlantManager::isCellOccupied(const sf::Vector2i &gridPosition) const
{
    for (const auto &plant : m_plants)
    {
        if (plant->getGridPosition() == gridPosition)
            return true;
    }
    return false;
}

const std::vector<std::unique_ptr<Plant>> &PlantManager::getAllPlants() const { return m_plants; }

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

void PlantManager::requestSunSpawnFromPlant(Plant *requestingPlant)
{
    if (requestingPlant)
    {
        m_gameStateRef.spawnSunFromPlant(requestingPlant); // 调用 GamePlayState 的方法
    }
}
