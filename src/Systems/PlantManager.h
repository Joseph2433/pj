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
class GamePlayState; // 前向声明

enum class PlantType
{
    SUNFLOWER,
    PEASHOOTER /*, ... */
};

class PlantManager
{
public:
    PlantManager(ResourceManager &resManager, Grid &gridSystem, GamePlayState &gameState); // 构造函数接收 GamePlayState
    ~PlantManager();

    bool tryAddPlant(PlantType type, const sf::Vector2i &gridPosition);
    void update(float dt);
    void draw(sf::RenderWindow &window);
    void clear();
    bool isCellOccupied(const sf::Vector2i &gridPosition) const;
    const std::vector<std::unique_ptr<Plant>> &getAllPlants() const;
    std::vector<Plant *> getPlantsInRow(int gridRow);

    void requestSunSpawnFromPlant(Plant *requestingPlant); // 新增

private:
    std::vector<std::unique_ptr<Plant>> m_plants;
    ResourceManager &m_resourceManagerRef;
    Grid &m_gridRef;
    GamePlayState &m_gameStateRef; // 持有 GamePlayState 引用
};
