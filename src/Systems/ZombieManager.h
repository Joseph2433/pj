#pragma once

#include <vector>
#include <memory>
#include <SFML/System.hpp>

namespace sf
{
    class RenderWindow;
}
class Zombie;
class ResourceManager;
class Grid;

// 僵尸类型
enum class ZombieType
{
    BASIC,
    BIG,
    BOSS,
    QUICK
};

class ZombieManager
{
public:
    ZombieManager(ResourceManager &resManager, Grid &grid);
    ~ZombieManager();
    void spawnZombie(int row, ZombieType type = ZombieType::BASIC);
    void update(float dt, const sf::RenderWindow &window);
    void draw(sf::RenderWindow &window);
    void clear();
    std::vector<Zombie *> getActiveZombies();

private:
    std::vector<std::unique_ptr<Zombie>> m_zombies;

    ResourceManager &m_resourceManagerRef;
    Grid &m_gridRef;
};
