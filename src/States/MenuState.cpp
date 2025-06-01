#include "MenuState.h"
#include "../Core/Game.h"
#include "../Utils/Constants.h"
#include <sstream>

MenuState::MenuState()
    : m_showGridInfo(true), m_mouseGridPos(-1, -1)
{
}

void MenuState::enter()
{
    // 获取资源管理器
    ResourceManager &resourceManager = Game::getInstance().getResourceManager();

    // 设置标题文本
    m_titleText.setString("Plants vs Zombies - Grid Test");
    if (resourceManager.hasFont("default"))
    {
        m_titleText.setFont(resourceManager.getFont("default"));
    }
    m_titleText.setCharacterSize(32);
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(50, 30);

    // 设置说明文本
    m_instructionText.setString("Move mouse over grid to see coordinates\nPress ESC to exit");
    if (resourceManager.hasFont("default"))
    {
        m_instructionText.setFont(resourceManager.getFont("default"));
    }
    m_instructionText.setCharacterSize(18);
    m_instructionText.setFillColor(sf::Color::Yellow);
    m_instructionText.setPosition(50, 80);

    // 设置网格信息文本
    if (resourceManager.hasFont("default"))
    {
        m_gridInfoText.setFont(resourceManager.getFont("default"));
    }
    m_gridInfoText.setCharacterSize(16);
    m_gridInfoText.setFillColor(sf::Color::Cyan);
    m_gridInfoText.setPosition(50, 130);
}

void MenuState::exit()
{
    // 清理资源
}

void MenuState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            Game::getInstance().getStateManager().popState();
        }
    }

    if (event.type == sf::Event::MouseMoved)
    {
        // 获取鼠标在网格中的位置
        Grid &grid = Game::getInstance().getGrid();
        sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x),
                              static_cast<float>(event.mouseMove.y));
        m_mouseGridPos = grid.getGridPosition(mousePos);
    }
}

void MenuState::update(float deltaTime)
{
    // 更新网格信息显示
    std::stringstream ss;
    ss << "Grid Info:\n";
    ss << "Size: " << GRID_ROWS << " rows x " << GRID_COLS << " columns\n";
    ss << "Cell Size: " << GRID_WIDTH << " x " << GRID_HEIGHT << " pixels\n";
    ss << "Grid Start: (" << GRID_START_X << ", " << GRID_START_Y << ")\n";

    if (m_mouseGridPos.x >= 0 && m_mouseGridPos.y >= 0)
    {
        Grid &grid = Game::getInstance().getGrid();
        sf::Vector2f worldPos = grid.getWorldPosition(m_mouseGridPos.x, m_mouseGridPos.y);
        ss << "\nMouse Grid Position: Row " << m_mouseGridPos.x << ", Col " << m_mouseGridPos.y;
        ss << "\nWorld Position: (" << static_cast<int>(worldPos.x) << ", " << static_cast<int>(worldPos.y) << ")";
    }
    else
    {
        ss << "\nMouse: Outside grid area";
    }

    m_gridInfoText.setString(ss.str());
}

void MenuState::render(sf::RenderWindow &window)
{
    // 渲染文本
    window.draw(m_titleText);
    window.draw(m_instructionText);
    window.draw(m_gridInfoText);

    // 如果鼠标在网格内，高亮显示当前格子
    if (m_mouseGridPos.x >= 0 && m_mouseGridPos.y >= 0)
    {
        Grid &grid = Game::getInstance().getGrid();
        sf::Vector2f worldPos = grid.getWorldPosition(m_mouseGridPos.x, m_mouseGridPos.y);
        sf::Vector2f cellSize = grid.getCellSize();

        sf::RectangleShape highlight;
        highlight.setSize(cellSize);
        highlight.setPosition(worldPos.x - cellSize.x / 2, worldPos.y - cellSize.y / 2);
        highlight.setFillColor(sf::Color(255, 255, 0, 50)); // 半透明黄色
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(2);

        window.draw(highlight);
    }
}