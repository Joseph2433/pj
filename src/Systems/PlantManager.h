#pragma once

#include <vector>
#include <memory>          // For std::unique_ptr
#include <SFML/System.hpp> // For sf::Vector2i (grid position)

// Forward Declarations
namespace sf
{
    class RenderWindow;
}
class Plant;           // Base class for all plants
class ResourceManager; // To pass to Plant constructors
class Grid;            // To pass to Plant constructors and for grid logic

// Enum to specify the type of plant to create
enum class PlantType
{
    SUNFLOWER,
    PEASHOOTER
    // Add more plant types here (e.g., WALLNUT, CHERRYBOMB)
};

class PlantManager
{
public:
    PlantManager(ResourceManager &resManager, Grid &gridSystem);
    ~PlantManager(); // Default destructor is fine with unique_ptr

    // Attempts to add a plant of the given type at the specified grid position.
    // gridPosition is expected to be (row, column).
    // Returns true if planting was successful, false otherwise (e.g., cell occupied, insufficient sun - sun check later).
    bool tryAddPlant(PlantType type, const sf::Vector2i &gridPosition);

    // Updates all active plants.
    // Removes plants that are no longer alive.
    void update(float dt);

    // Draws all active plants to the window.
    void draw(sf::RenderWindow &window);

    // Removes all plants from the manager (e.g., for game reset).
    void clear();

    // Checks if a specific grid cell is already occupied by a plant.
    // gridPosition is expected to be (row, column).
    bool isCellOccupied(const sf::Vector2i &gridPosition) const;

    // Returns a constant reference to the vector of all plants.
    // Useful for systems that need to iterate over all plants (e.g., collision detection).
    const std::vector<std::unique_ptr<Plant>> &getAllPlants() const;

    // Returns a vector of raw pointers to plants in a specific row.
    // Useful for zombies to find targets or peashooters to check for zombies.
    // The caller does not own these pointers; their lifetime is managed by PlantManager.
    // gridRow is the row index.
    std::vector<Plant *> getPlantsInRow(int gridRow);

private:
    std::vector<std::unique_ptr<Plant>> m_plants; // Stores all active plant objects
    ResourceManager &m_resourceManager;           // Reference to the game's resource manager
    Grid &m_grid;                                 // Reference to the game's grid system
    // SunManager* m_sunManager; // Future: reference to SunManager for cost checking
};
