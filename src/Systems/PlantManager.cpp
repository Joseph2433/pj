#include "PlantManager.h"
#include "../Entities/Plant.h"
#include "../Plants/Sunflower.h"
#include "../Plants/Peashooter.h"
#include "../Plants/WallNut.h"
#include "../Plants/IcePeashooter.h"
#include "../Core/ResourceManager.h"
#include "../Systems/Grid.h"
#include "../States/GamePlayState.h"
#include "../Systems/ProjectileManager.h"
#include "../Systems/ZombieManager.h"
#include "../Entities/Zombie.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <algorithm>
#include <iostream>

PlantManager::PlantManager(ResourceManager &resManager, Grid &gridSystem,
                           GamePlayState &gameState, ProjectileManager &projectileManager, ZombieManager &zombieManager)
    : m_resourceManagerRef(resManager),
      m_gridRef(gridSystem),
      m_gameStateRef(gameState),
      m_projectileManagerRef_forPlants(projectileManager),
      m_zombieManagerRef(zombieManager)
{
}

PlantManager::~PlantManager() = default;

// sunflower
std::unique_ptr<Plant> PlantManager::createSunflower(const sf::Vector2i &gridPosition)
{
    return std::make_unique<Sunflower>(m_resourceManagerRef, gridPosition, m_gridRef, *this);
}

// peashooter
std::unique_ptr<Plant> PlantManager::createPeashooter(const sf::Vector2i &gridPosition)
{
    return std::make_unique<Peashooter>(m_resourceManagerRef, gridPosition, m_gridRef, *this, m_projectileManagerRef_forPlants);
}

// wallnut
std::unique_ptr<Plant> PlantManager::createWallNut(const sf::Vector2i &gridPosition)
{
    return std::make_unique<WallNut>(m_resourceManagerRef, gridPosition, m_gridRef);
}

// icepeashooter
std::unique_ptr<Plant> PlantManager::createIcePeashooter(const sf::Vector2i &gridPosition)
{

    return std::make_unique<IcePeashooter>(m_resourceManagerRef, gridPosition, m_gridRef, *this, m_projectileManagerRef_forPlants);
}

std::vector<Zombie *> PlantManager::getZombiesInLane(int lane) const
{

    std::vector<Zombie *> zombiesInSpecifiedLane;
    for (Zombie *zombie : m_zombieManagerRef.getActiveZombies())
    {
        if (zombie && zombie->isAlive())
        {
            int zombieCurrentLane = zombie->getLane();
            if (zombieCurrentLane == lane)
            {
                zombiesInSpecifiedLane.push_back(zombie);
            }
        }
    }
    return zombiesInSpecifiedLane;
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
        newPlant = createSunflower(gridPosition);
        break;
    case PlantType::PEASHOOTER:
        newPlant = createPeashooter(gridPosition);
        break;
    case PlantType::WALLNUT:
        newPlant = createWallNut(gridPosition);
        break;
    case PlantType::ICEPEASHOOTER:
        newPlant = createIcePeashooter(gridPosition);
        break;
    default:
        std::cerr << "PlantManager: undefined error, plant is " << static_cast<int>(type) << std::endl;
        return false;
    }

    if (newPlant)
    {
        // (在这里加入阳光花费检查，例如：)
        // if (m_gameStateRef.getSunManager().trySpendSun(newPlant->getCost())) {
        //     m_plants.push_back(std::move(newPlant));
        //     return true;
        // } else {
        //     std::cout << "阳光不足以种植此植物！" << std::endl;
        //     return false;
        // }
        m_plants.push_back(std::move(newPlant));
        std::cout << "PlantManager: planted " << static_cast<int>(type) << " in  (" << gridPosition.x << ", " << gridPosition.y << ")" << std::endl;

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

std::vector<Plant *> PlantManager::getAllActivePlants()
{
    std::vector<Plant *> activePlants;
    for (const auto &plant_ptr : m_plants)
    {
        if (plant_ptr && plant_ptr->isAlive())
        {
            activePlants.push_back(plant_ptr.get());
        }
    }
    return activePlants;
}

void PlantManager::requestSunSpawnFromPlant(Plant *requestingPlant)
{
    if (requestingPlant)
    {
        m_gameStateRef.spawnSunFromPlant(requestingPlant);
    }
}

Plant *PlantManager::getPlantAt(const sf::Vector2i &gridPosition)
{
    for (auto &plant_ptr : m_plants)
    {
        if (plant_ptr && plant_ptr->getGridPosition() == gridPosition && plant_ptr->isAlive())
        {
            return plant_ptr.get();
        }
    }
    return nullptr;
}

bool PlantManager::removePlant(Plant *plantToRemove)
{
    if (!plantToRemove)
        return false;

    auto it = std::find_if(m_plants.begin(), m_plants.end(),
                           [&](const std::unique_ptr<Plant> &p)
                           { return p.get() == plantToRemove; });

    if (it != m_plants.end())
    {
        sf::Vector2i gridPos = (*it)->getGridPosition();
        m_plants.erase(it);
        if (m_gridRef.isValidGridPosition(gridPos))
        {
            m_gridRef.setCellOccupied(gridPos.x, gridPos.y, false);
            std::cout << "PlantManager: Plant removed from grid (" << gridPos.x << "," << gridPos.y
                      << "), cell now unoccupied in Grid." << std::endl;
        }
        return true;
    }
    return false;
}

bool PlantManager::removePlantAt(const sf::Vector2i &gridPosition)
{
    Plant *plant = getPlantAt(gridPosition);
    if (plant)
    {
        return removePlant(plant);
    }
    return false;
}