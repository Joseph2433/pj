// GamePlayState.cpp - 方案二：修正背景和网格
#include "States/GamePlayState.h"
#include "States/MenuState.h"
#include "Core/StateManager.h"
#include "../Utils/Constants.h"
#include <iostream>
#include <sstream>

GamePlayState::GamePlayState(StateManager *stateManager)
    : GameState(stateManager),
      m_useCustomFont(false),
      m_gameTime(0.0f),
      m_mousePos(0, 0)
{
}

void GamePlayState::enter()
{
    // 尝试加载字体
    bool fontLoaded = false;

    // 尝试加载系统字体
    if (m_font.loadFromFile("C:/Windows/Fonts/arial.ttf") ||
        m_font.loadFromFile("C:/Windows/Fonts/msyh.ttc"))
    {
        fontLoaded = true;
    }

    m_useCustomFont = fontLoaded;

    // 设置背景覆盖整个窗口
    m_background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT)); // 使用窗口常量
    m_background.setFillColor(sf::Color(34, 139, 34));               // 森林绿
    m_background.setPosition(0, 0);                                  // 确保从左上角开始

    // 设置标题
    m_titleText.setString("Game Play - Plants vs Zombies");
    if (m_useCustomFont)
    {
        m_titleText.setFont(m_font);
    }
    m_titleText.setCharacterSize(32);
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(50, 30);

    // 设置说明文本
    // m_instructionText.setString("Game Started! Move mouse to see position\nPress ESC to return to menu\nPress R to restart game");
    // if (m_useCustomFont)
    // {
    //     m_instructionText.setFont(m_font);
    // }
    // m_instructionText.setCharacterSize(18);
    // m_instructionText.setFillColor(sf::Color::Yellow);
    // m_instructionText.setPosition(50, 80);

    // 设置游戏信息文本
    if (m_useCustomFont)
    {
        m_gameInfoText.setFont(m_font);
    }
    m_gameInfoText.setCharacterSize(16);
    m_gameInfoText.setFillColor(sf::Color::Cyan);
    m_gameInfoText.setPosition(50, 150);

    m_gameTime = 0.0f;

    std::cout << "GamePlayState entered successfully!" << std::endl;
}

void GamePlayState::exit()
{
    std::cout << "GamePlayState exited" << std::endl;
}

void GamePlayState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Escape:
            // 返回主菜单
            m_stateManager->changeState(std::make_unique<MenuState>(m_stateManager));
            break;
        case sf::Keyboard::R:
            // 重启游戏
            m_gameTime = 0.0f;
            std::cout << "Game restarted!" << std::endl;
            break;
        default:
            break;
        }
    }

    if (event.type == sf::Event::MouseMoved)
    {
        m_mousePos.x = event.mouseMove.x;
        m_mousePos.y = event.mouseMove.y;
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            std::cout << "Mouse clicked at: (" << event.mouseButton.x << ", " << event.mouseButton.y << ")" << std::endl;
        }
    }
}

void GamePlayState::update(float deltaTime)
{
    m_gameTime += deltaTime;

    // 更新游戏信息显示
    std::stringstream ss;
    ss << "Game Info:\n";
    ss << "Game Time: " << static_cast<int>(m_gameTime) << " seconds\n";
    ss << "Mouse Position: (" << m_mousePos.x << ", " << m_mousePos.y << ")\n";
    ss << "FPS: " << static_cast<int>(1.0f / deltaTime) << "\n";
    ss << "\nThis is where the actual game logic would go:\n";
    ss << "- Plant placement\n";
    ss << "- Zombie spawning and movement\n";
    ss << "- Collision detection\n";
    ss << "- Game mechanics";

    m_gameInfoText.setString(ss.str());
}

void GamePlayState::render(sf::RenderWindow &window)
{
    // 渲染背景（覆盖整个窗口）
    window.draw(m_background);

    // 渲染标题
    window.draw(m_titleText);

    // 渲染说明文本
    window.draw(m_instructionText);

    // 渲染游戏信息
    window.draw(m_gameInfoText);

    // 绘制改进的网格来模拟游戏区域
    sf::RectangleShape gridLine;
    gridLine.setFillColor(sf::Color(0, 200, 0, 150)); // 更亮的绿色，提高可见度

    // 计算网格总宽度和高度（包含边框）
    const int GRID_TOTAL_WIDTH = GRID_COLS * GRID_WIDTH + (GRID_COLS + 1) * CELL_PADDING;
    const int GRID_TOTAL_HEIGHT = GRID_ROWS * GRID_HEIGHT + (GRID_ROWS + 1) * CELL_PADDING;

    // 计算网格起始位置（居中显示，或自定义边距）
    const int GRID_START_X = (WINDOW_WIDTH - GRID_TOTAL_WIDTH) / 2; // 水平居中
    const int GRID_START_Y = 200;                                   // 顶部边距200px

    // 定义线条样式（使用单独的线条宽度变量，可灵活调整）
    const int LINE_WIDTH = CELL_PADDING * 2;          // 线条宽度等于2倍间距，视觉更明显
    gridLine.setFillColor(sf::Color(0, 200, 0, 180)); // 深绿色半透明线条
    gridLine.setOutlineThickness(0);                  // 禁用轮廓

    // 绘制垂直线（列边框）
    for (int col = 0; col <= GRID_COLS; col++)
    {
        int x = GRID_START_X + col * (GRID_WIDTH + CELL_PADDING);
        gridLine.setSize(sf::Vector2f(LINE_WIDTH, GRID_TOTAL_HEIGHT));
        gridLine.setPosition(x, GRID_START_Y);
        window.draw(gridLine);
    }

    // 绘制水平线（行边框）
    for (int row = 0; row <= GRID_ROWS; row++)
    {
        int y = GRID_START_Y + row * (GRID_HEIGHT + CELL_PADDING);
        gridLine.setSize(sf::Vector2f(GRID_TOTAL_WIDTH, LINE_WIDTH));
        gridLine.setPosition(GRID_START_X, y);
        window.draw(gridLine);
    }

    // 优化后的鼠标位置指示器（适配网格坐标）
    if (m_mousePos.x >= 0 && m_mousePos.y >= 0)
    {
        // 计算鼠标所在网格单元格（可选功能）
        int mouseCol = (m_mousePos.x - GRID_START_X - LINE_WIDTH / 2) / (GRID_WIDTH + CELL_PADDING);
        int mouseRow = (m_mousePos.y - GRID_START_Y - LINE_WIDTH / 2) / (GRID_HEIGHT + CELL_PADDING);

        sf::CircleShape mouseIndicator(6); // 更大的指示器
        mouseIndicator.setFillColor(sf::Color::Red);
        mouseIndicator.setOutlineColor(sf::Color::White);
        mouseIndicator.setOutlineThickness(3);
        mouseIndicator.setPosition(m_mousePos.x - 12, m_mousePos.y - 12); // 居中显示

        window.draw(mouseIndicator);
    }
}