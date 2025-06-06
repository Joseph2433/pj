#pragma once

#include <vector>
#include <memory>
#include <SFML/System.hpp>

// 前向声明
namespace sf
{
    class RenderWindow;
}
class Plant;
class ResourceManager;
class Grid;
class GamePlayState; // 用于阳光生成回调
class ProjectileManager;
class ZombieManager;
class Zombie;

enum class PlantType
{
    SUNFLOWER,
    PEASHOOTER,
    WALLNUT
    // ... 其他植物类型
};

class PlantManager
{
public:
    // 构造函数现在需要 GamePlayState (用于产阳光) 和 ProjectileManager (用于发射子弹的植物)
    PlantManager(ResourceManager &resManager, Grid &gridSystem,
                 GamePlayState &gameState, ProjectileManager &projectileManager, ZombieManager &zombieManager);
    ~PlantManager(); // 在.cpp中定义，确保Plant类型完整

    // 尝试在指定网格位置种植植物
    bool tryAddPlant(PlantType type, const sf::Vector2i &gridPosition);
    void update(float dt);
    void draw(sf::RenderWindow &window);
    void clear();
    bool isCellOccupied(const sf::Vector2i &gridPosition) const;
    const std::vector<std::unique_ptr<Plant>> &getAllPlants() const;
    std::vector<Plant *> getPlantsInRow(int gridRow);
    std::vector<Plant *> getAllActivePlants(); // 返回原始指针列表

    // 供植物（如向日葵）调用以请求在其位置产生阳光
    void requestSunSpawnFromPlant(Plant *requestingPlant);
    std::vector<Zombie *> getZombiesInLane(int lane) const; // 供植物索敌

    Plant *getPlantAt(const sf::Vector2i &gridPosition); // <--- 新增：获取指定格子的植物
    bool removePlant(Plant *plantToRemove);              // <--- 新增：移除指定的植物对象
    bool removePlantAt(const sf::Vector2i &gridPosition);

private:
    // 私有创建辅助方法，用于根据植物类型调用正确的构造函数
    std::unique_ptr<Plant> createSunflower(const sf::Vector2i &gridPosition);
    std::unique_ptr<Plant> createPeashooter(const sf::Vector2i &gridPosition);
    std::unique_ptr<Plant> createWallNut(const sf::Vector2i &gridPosition);

    std::vector<std::unique_ptr<Plant>> m_plants; // 存储所有植物
    ResourceManager &m_resourceManagerRef;        // 资源管理器引用
    Grid &m_gridRef;                              // 网格系统引用
    GamePlayState &m_gameStateRef;                // GamePlayState引用 (用于回调)
    ProjectileManager &m_projectileManagerRef_forPlants;
    ZombieManager &m_zombieManagerRef;
};
