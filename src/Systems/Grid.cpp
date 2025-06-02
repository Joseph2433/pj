#include "Grid.h"
#include "../Utils/Constants.h"

Grid::Grid()
    : m_rows(GRID_ROWS), m_cols(GRID_COLS), m_cellWidth(GRID_CELL_WIDTH), m_cellHeight(GRID_CELL_HEIGHT), m_startPosition(GRID_START_X, GRID_START_Y)
{

    // 初始化占用状态数组
    m_occupiedCells.resize(m_rows);
    for (int i = 0; i < m_rows; ++i)
    {
        m_occupiedCells[i].resize(m_cols, false);
    }
}

void Grid::initialize()
{
    createGridLines();
}

void Grid::createGridLines()
{
    // 创建水平线
    m_horizontalLines.clear();
    for (int i = 0; i <= m_rows; ++i)
    {
        sf::RectangleShape line;
        line.setSize(sf::Vector2f(m_cols * m_cellWidth, 1));
        line.setPosition(m_startPosition.x, m_startPosition.y + i * m_cellHeight);
        line.setFillColor(sf::Color(GRID_LINE_COLOR_R, GRID_LINE_COLOR_G,
                                    GRID_LINE_COLOR_B, GRID_LINE_COLOR_A));
        m_horizontalLines.push_back(line);
    }

    // 创建垂直线
    m_verticalLines.clear();
    for (int i = 0; i <= m_cols; ++i)
    {
        sf::RectangleShape line;
        line.setSize(sf::Vector2f(1, m_rows * m_cellHeight));
        line.setPosition(m_startPosition.x + i * m_cellWidth, m_startPosition.y);
        line.setFillColor(sf::Color(GRID_LINE_COLOR_R, GRID_LINE_COLOR_G,
                                    GRID_LINE_COLOR_B, GRID_LINE_COLOR_A));
        m_verticalLines.push_back(line);
    }
}

void Grid::render(sf::RenderWindow &window)
{
    // 渲染水平线
    for (const auto &line : m_horizontalLines)
    {
        window.draw(line);
    }

    // 渲染垂直线
    for (const auto &line : m_verticalLines)
    {
        window.draw(line);
    }
}

sf::Vector2f Grid::getWorldPosition(int row, int col) const
{
    if (!isValidGridPosition(row, col))
    {
        return sf::Vector2f(-1, -1); // 无效位置
    }

    float x = m_startPosition.x + col * m_cellWidth + m_cellWidth / 2;
    float y = m_startPosition.y + row * m_cellHeight + m_cellHeight / 2;
    return sf::Vector2f(x, y);
}

sf::Vector2i Grid::getGridPosition(float x, float y) const
{
    return getGridPosition(sf::Vector2f(x, y));
}

sf::Vector2i Grid::getGridPosition(const sf::Vector2f &worldPos) const
{
    // 检查是否在网格范围内
    if (worldPos.x < m_startPosition.x || worldPos.y < m_startPosition.y ||
        worldPos.x > m_startPosition.x + m_cols * m_cellWidth ||
        worldPos.y > m_startPosition.y + m_rows * m_cellHeight)
    {
        return sf::Vector2i(-1, -1); // 无效位置
    }

    int col = static_cast<int>((worldPos.x - m_startPosition.x) / m_cellWidth);
    int row = static_cast<int>((worldPos.y - m_startPosition.y) / m_cellHeight);

    // 确保在有效范围内
    col = std::max(0, std::min(col, m_cols - 1));
    row = std::max(0, std::min(row, m_rows - 1));

    return sf::Vector2i(row, col);
}

bool Grid::isValidGridPosition(int row, int col) const
{
    return row >= 0 && row < m_rows && col >= 0 && col < m_cols;
}

bool Grid::isValidGridPosition(const sf::Vector2i &gridPos) const
{
    return isValidGridPosition(gridPos.x, gridPos.y);
}

bool Grid::isCellOccupied(int row, int col) const
{
    if (!isValidGridPosition(row, col))
    {
        return true; // 无效位置视为被占用
    }
    return m_occupiedCells[row][col];
}

void Grid::setCellOccupied(int row, int col, bool occupied)
{
    if (isValidGridPosition(row, col))
    {
        m_occupiedCells[row][col] = occupied;
    }
}

int Grid::getRows() const
{
    return m_rows;
}

int Grid::getCols() const
{
    return m_cols;
}

sf::Vector2f Grid::getCellSize() const
{
    return sf::Vector2f(m_cellWidth, m_cellHeight);
}

sf::Vector2f Grid::getGridStartPosition() const
{
    return m_startPosition;
}