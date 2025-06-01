#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>
#include <array>
#include <memory>
#include "../Utils/Constants.h"

// 前向声明
class Plant;
class Zombie;

// 网格单元格类，表示草坪上的一个位置
class GridCell
{
public:
    GridCell();

    // 植物管理
    void setPlant(std::shared_ptr<Plant> plant) { m_plant = plant; }
    std::shared_ptr<Plant> getPlant() const { return m_plant; }
    bool hasPlant() const { return m_plant != nullptr; }
    void removePlant() { m_plant = nullptr; }

    // 僵尸管理（一个格子可能有多个僵尸经过）
    void addZombie(std::shared_ptr<Zombie> zombie);
    void removeZombie(std::shared_ptr<Zombie> zombie);
    const std::vector<std::shared_ptr<Zombie>> &getZombies() const { return m_zombies; }
    bool hasZombies() const { return !m_zombies.empty(); }

    // 格子类型（普通草地、游泳池等）
    enum CellType
    {
        GRASS,
        POOL
    };
    void setCellType(CellType type) { m_cellType = type; }
    CellType getCellType() const { return m_cellType; }

private:
    std::shared_ptr<Plant> m_plant;                 // 这个格子上的植物
    std::vector<std::shared_ptr<Zombie>> m_zombies; // 经过这个格子的僵尸
    CellType m_cellType;                            // 格子类型
};

// Grid类管理整个9x5的游戏网格
class Grid
{
public:
    Grid();

    // 网格访问
    GridCell &getCell(int x, int y);
    const GridCell &getCell(int x, int y) const;
    bool isValidPosition(int x, int y) const;

    // 坐标转换
    sf::Vector2f gridToWorldPosition(int gridX, int gridY) const;
    sf::Vector2i worldToGridPosition(const sf::Vector2f &worldPos) const;

    // 植物操作
    bool canPlantAt(int x, int y) const;
    bool plantAt(int x, int y, std::shared_ptr<Plant> plant);
    void removePlantAt(int x, int y);

    // 渲染网格（调试用）
    void renderGrid(sf::RenderWindow &window) const;

private:
    // 使用二维数组存储网格单元格
    std::array<std::array<GridCell, GRID_ROWS>, GRID_COLS> m_cells;

    // 网格可视化（调试模式）
    mutable sf::RectangleShape m_cellOutline;
    bool m_showGrid;
};

#endif