#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Grid
{
public:
    Grid();
    ~Grid() = default;

    // 初始化网格
    void initialize();

    // 渲染网格线
    void render(sf::RenderWindow &window);

    // 坐标转换
    sf::Vector2f getWorldPosition(int row, int col) const;
    sf::Vector2i getGridPosition(float x, float y) const;
    sf::Vector2i getGridPosition(const sf::Vector2f &worldPos) const;

    // 网格验证
    bool isValidGridPosition(int row, int col) const;
    bool isValidGridPosition(const sf::Vector2i &gridPos) const;

    // 网格状态（用于检查是否可以放置物体）
    bool isCellOccupied(int row, int col) const;
    void setCellOccupied(int row, int col, bool occupied);

    // 获取网格信息
    int getRows() const;
    int getCols() const;
    sf::Vector2f getCellSize() const;
    sf::Vector2f getGridStartPosition() const;

private:
    void createGridLines();

    std::vector<sf::RectangleShape> m_horizontalLines;
    std::vector<sf::RectangleShape> m_verticalLines;
    std::vector<std::vector<bool>> m_occupiedCells;

    int m_rows;
    int m_cols;
    float m_cellWidth;
    float m_cellHeight;
    sf::Vector2f m_startPosition;
};