#include "Grid.h"
#include "../Entities/Plant.h"
#include "../Entities/Zombie.h"

GridCell::GridCell()
    : m_cellType(GRASS)
{
}

void GridCell::addZombie(std::shared_ptr<Zombie> zombie)
{
    m_zombies.push_back(zombie);
}

void GridCell::removeZombie(std::shared_ptr<Zombie> zombie)
{
    auto it = std::find(m_zombies.begin(), m_zombies.end(), zombie);
    if (it != m_zombies.end())
    {
        m_zombies.erase(it);
    }
}

Grid::Grid() : m_showGrid(false)
{
    // 初始化网格可视化
    m_cellOutline.setSize(sf::Vector2f(GRID_CELL_WIDTH - 1, GRID_CELL_HEIGHT - 1));
    m_cellOutline.setFillColor(sf::Color::Transparent);
    m_cellOutline.setOutlineColor(sf::Color(100, 100, 100, 100));
    m_cellOutline.setOutlineThickness(1);

    // 设置特殊格子类型（比如游泳池）
    // 在经典PVZ中，第3和第4行是游泳池
    for (int x = 0; x < GRID_COLS; ++x)
    {
        m_cells[x][2].setCellType(GridCell::POOL); // 第3行
        m_cells[x][3].setCellType(GridCell::POOL); // 第4行
    }
}

GridCell &Grid::getCell(int x, int y)
{
    return m_cells[x][y];
}

const GridCell &Grid::getCell(int x, int y) const
{
    return m_cells[x][y];
}

bool Grid::isValidPosition(int x, int y) const
{
    return x >= 0 && x < GRID_COLS && y >= 0 && y < GRID_ROWS;
}

sf::Vector2f Grid::gridToWorldPosition(int gridX, int gridY) const
{
    return sf::Vector2f(
        GRID_OFFSET_X + gridX * GRID_CELL_WIDTH,
        GRID_OFFSET_Y + gridY * GRID_CELL_HEIGHT);
}

sf::Vector2i Grid::worldToGridPosition(const sf::Vector2f &worldPos) const
{
    int gridX = static_cast<int>((worldPos.x - GRID_OFFSET_X) / GRID_CELL_WIDTH);
    int gridY = static_cast<int>((worldPos.y - GRID_OFFSET_Y) / GRID_CELL_HEIGHT);
    return sf::Vector2i(gridX, gridY);
}

bool Grid::canPlantAt(int x, int y) const
{
    if (!isValidPosition(x, y))
        return false;

    const GridCell &cell = getCell(x, y);

    // 不能在已有植物的地方种植
    if (cell.hasPlant())
        return false;

    // 某些植物不能种在游泳池里（这里简化处理）
    // 实际游戏中需要检查植物类型
    return true;
}

bool Grid::plantAt(int x, int y, std::shared_ptr<Plant> plant)
{
    if (!canPlantAt(x, y))
        return false;

    getCell(x, y).setPlant(plant);
    return true;
}

void Grid::removePlantAt(int x, int y)
{
    if (isValidPosition(x, y))
    {
        getCell(x, y).removePlant();
    }
}

void Grid::renderGrid(sf::RenderWindow &window) const
{
    if (!m_showGrid)
        return;

    // 绘制网格线，用于调试
    for (int x = 0; x < GRID_COLS; ++x)
    {
        for (int y = 0; y < GRID_ROWS; ++y)
        {
            sf::Vector2f pos = gridToWorldPosition(x, y);
            m_cellOutline.setPosition(pos);

            // 根据格子类型设置不同颜色
            const GridCell &cell = getCell(x, y);
            if (cell.getCellType() == GridCell::POOL)
            {
                m_cellOutline.setOutlineColor(sf::Color(0, 100, 200, 100));
            }
            else
            {
                m_cellOutline.setOutlineColor(sf::Color(100, 100, 100, 100));
            }

            window.draw(m_cellOutline);
        }
    }
}