#include "PlantManager.h"
#include "../Entities/Plant.h"   // Base Plant class
#include "../Plants/Sunflower.h" // Specific plant implementations
#include "../Plants/Peashooter.h"
// #include "../Plants/WallNut.h"    // Example for future plants
#include "../Core/ResourceManager.h" // Ensure correct paths
#include "../Systems/Grid.h"         // Ensure correct paths
// #include "../GameLogic/SunManager.h" // For future sun cost checking

#include <SFML/Graphics/RenderWindow.hpp>
#include <algorithm> // For std::remove_if
#include <iostream>  // For debugging (optional)

PlantManager::PlantManager(ResourceManager &resManager, Grid &gridSystem)
    : m_resourceManager(resManager), m_grid(gridSystem)
{
    // m_sunManager = nullptr; // Initialize if you add SunManager
}

PlantManager::~PlantManager()
{
    // m_plants 中的 unique_ptr 会在这里自动被销毁，
    // 从而调用 Plant 的析构函数。
    // 如果 Plant 的析构函数不是虚函数，并且你通过 Plant* 删除派生类，也会有问题，
    // 但 Entity 基类应该已经有虚析构函数了。
    clear(); // 主动调用 clear 也是一种确保清理的方式，或者让 vector 自动处理
    std::cout << "PlantManager destroyed." << std::endl;
}
// PlantManager::~PlantManager() = default;

bool PlantManager::isCellOccupied(const sf::Vector2i &gridPosition) const
{
    // gridPosition.x is row, gridPosition.y is column
    for (const auto &plant : m_plants)
    {
        if (plant->getGridPosition() == gridPosition)
        {
            return true;
        }
    }
    return false;
}

bool PlantManager::tryAddPlant(PlantType type, const sf::Vector2i &gridPosition)
{
    // gridPosition.x is row, gridPosition.y is column

    // 1. Check if the grid position is valid (within grid bounds)
    //    The Plant constructor itself doesn't do this check, relying on the caller.
    //    The Grid class has isValidGridPosition.
    if (!m_grid.isValidGridPosition(gridPosition))
    {
        // std::cout << "PlantManager: Attempt to plant outside grid at ("
        //           << gridPosition.x << "r, " << gridPosition.y << "c)" << std::endl;
        return false;
    }

    // 2. Check if the cell is already occupied
    if (isCellOccupied(gridPosition))
    {
        // std::cout << "PlantManager: Cell (" << gridPosition.x << "r, "
        //           << gridPosition.y << "c) is already occupied." << std::endl;
        return false;
    }

    // 3. Create the plant based on type
    std::unique_ptr<Plant> newPlant = nullptr;
    int plantCost = 0; // To store the cost for sun check later

    switch (type)
    {
    case PlantType::SUNFLOWER:
        newPlant = std::make_unique<Sunflower>(m_resourceManager, gridPosition, m_grid);
        // plantCost = Constants::SUNFLOWER_COST; // Or get from newPlant->getCost() after creation
        break;
    case PlantType::PEASHOOTER:
        newPlant = std::make_unique<Peashooter>(m_resourceManager, gridPosition, m_grid);
        // plantCost = Constants::PEASHOOTER_COST;
        break;
    // Add cases for other plant types here
    // case PlantType::WALLNUT:
    //     newPlant = std::make_unique<WallNut>(m_resourceManager, gridPosition, m_grid);
    //     break;
    default:
        std::cerr << "PlantManager: Unknown plant type requested!" << std::endl;
        return false;
    }

    if (!newPlant)
    { // Should not happen if switch case is exhaustive for enum
        return false;
    }

    // 4. (Future) Check for sufficient sun
    // if (m_sunManager && m_sunManager->getCurrentSun() < newPlant->getCost()) { // Use newPlant->getCost()
    //     std::cout << "PlantManager: Not enough sun to plant " << static_cast<int>(type)
    //               << ". Need " << newPlant->getCost() << std::endl;
    //     return false;
    // }

    // 5. If all checks pass, add the plant
    // if (m_sunManager) {
    //     m_sunManager->spendSun(newPlant->getCost());
    // }
    m_plants.push_back(std::move(newPlant));
    // std::cout << "PlantManager: Planted type " << static_cast<int>(type)
    //           << " at (" << gridPosition.x << "r, " << gridPosition.y << "c)" << std::endl;
    return true;
}

void PlantManager::update(float dt)
{
    // Update all plants
    for (auto &plant : m_plants)
    {
        plant->update(dt);
    }

    // Remove dead plants (plants whose health <= 0)
    // The erase-remove idiom is efficient for this.
    m_plants.erase(
        std::remove_if(m_plants.begin(), m_plants.end(),
                       [](const std::unique_ptr<Plant> &p)
                       {
                           return !p->isAlive(); // Remove if not alive
                       }),
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
    m_plants.clear(); // This will call destructors of all contained unique_ptrs,
                      // which in turn call Plant destructors.
    // std::cout << "PlantManager: All plants cleared." << std::endl;
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
        // Plant::getGridPosition() returns (row, column)
        // Plant::getRow() returns plantPtr->getGridPosition().x
        if (plantPtr->isAlive() && plantPtr->getRow() == gridRow)
        {
            plantsInRow.push_back(plantPtr.get()); // Get raw pointer from unique_ptr
        }
    }
    return plantsInRow;
}
