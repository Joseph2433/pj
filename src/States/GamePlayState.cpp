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
    m_instructionText.setString("Game Started! Move mouse to see position\nPress ESC to return to menu\nPress R to restart game");
    if (m_useCustomFont)
    {
        m_instructionText.setFont(m_font);
    }
    m_instructionText.setCharacterSize(18);
    m_instructionText.setFillColor(sf::Color::Yellow);
    m_instructionText.setPosition(50, 80);

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

    // 网格参数
    const int gridStartX = 50;
    const int gridStartY = 300;
    const int gridWidth = WINDOW_WIDTH - 100;   // 窗口宽度 - 边距
    const int gridHeight = WINDOW_HEIGHT - 350; // 窗口高度 - 顶部UI区域
    const int cellWidth = 80;
    const int cellHeight = 80;
    const int lineWidth = 3; // 更粗的线条

    // 绘制垂直线
    for (int x = gridStartX; x <= gridStartX + gridWidth; x += cellWidth)
    {
        gridLine.setSize(sf::Vector2f(lineWidth, gridHeight));
        gridLine.setPosition(x, gridStartY);
        window.draw(gridLine);
    }

    // 绘制水平线
    for (int y = gridStartY; y <= gridStartY + gridHeight; y += cellHeight)
    {
        gridLine.setSize(sf::Vector2f(gridWidth, lineWidth));
        gridLine.setPosition(gridStartX, y);
        window.draw(gridLine);
    }

    // 绘制鼠标位置指示器
    if (m_mousePos.x >= 0 && m_mousePos.y >= 0)
    {
        sf::CircleShape mouseIndicator(8); // 更大的指示器
        mouseIndicator.setFillColor(sf::Color::Red);
        mouseIndicator.setOutlineColor(sf::Color::White);
        mouseIndicator.setOutlineThickness(2);
        mouseIndicator.setPosition(m_mousePos.x - 8, m_mousePos.y - 8);
        window.draw(mouseIndicator);
    }
}