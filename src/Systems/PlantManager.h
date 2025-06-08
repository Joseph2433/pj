#pragma once

#include <vector>
#include <memory>
#include <SFML/System.hpp>

namespace sf
{
    class RenderWindow;
}
class Plant;
class ResourceManager;
class Grid;
class GamePlayState;
class ProjectileManager;
class ZombieManager;
class Zombie;

enum class PlantType
{
    SUNFLOWER,
    PEASHOOTER,
    WALLNUT,
    ICEPEASHOOTER
};

class PlantManager
{
public:
    PlantManager(ResourceManager &resManager, Grid &gridSystem,
                 GamePlayState &gameState, ProjectileManager &projectileManager, ZombieManager &zombieManager);
    ~PlantManager();

    // 尝试在指定网格位置种植植物
    bool tryAddPlant(PlantType type, const sf::Vector2i &gridPosition);
    void update(float dt);
    void draw(sf::RenderWindow &window);
    void clear();
    bool isCellOccupied(const sf::Vector2i &gridPosition) const;
    const std::vector<std::unique_ptr<Plant>> &getAllPlants() const;
    std::vector<Plant *> getPlantsInRow(int gridRow);
    std::vector<Plant *> getAllActivePlants();

    // 供植物（如向日葵）调用以请求在其位置产生阳光
    void requestSunSpawnFromPlant(Plant *requestingPlant);
    std::vector<Zombie *> getZombiesInLane(int lane) const;

    Plant *getPlantAt(const sf::Vector2i &gridPosition);
    bool removePlant(Plant *plantToRemove);
    bool removePlantAt(const sf::Vector2i &gridPosition);

private:
    std::unique_ptr<Plant> createSunflower(const sf::Vector2i &gridPosition);
    std::unique_ptr<Plant> createPeashooter(const sf::Vector2i &gridPosition);
    std::unique_ptr<Plant> createWallNut(const sf::Vector2i &gridPosition);
    std::unique_ptr<Plant> createIcePeashooter(const sf::Vector2i &gridPosition);

    std::vector<std::unique_ptr<Plant>> m_plants;
    ResourceManager &m_resourceManagerRef;
    Grid &m_gridRef;
    GamePlayState &m_gameStateRef;
    ProjectileManager &m_projectileManagerRef_forPlants;
    ZombieManager &m_zombieManagerRef;
};
