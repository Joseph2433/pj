#include "Game.h"
#include "States/MenuState.h"
#include "States/GamePlayState.h"
#include "../Utils/Constants.h"
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Default),
      m_resourceManager(),
      m_stateManager(this)
{
    m_window.setFramerateLimit(60);
    std::cout << "Game object constructed." << std::endl;
    std::cout << "Window created with size: " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;

    // 设置初始状态为 MenuState
    std::cout << "Pushing initial MenuState..." << std::endl;
    m_stateManager.pushState(std::make_unique<MenuState>(&m_stateManager));

    if (m_stateManager.isEmpty())
    {
        std::cerr << "Error: Failed to push initial state. StateManager is empty." << std::endl;
        m_window.close();
    }
    else
    {
        std::cout << "Initial state (MenuState) pushed successfully." << std::endl;
    }
}

void Game::run()
{
    sf::Clock clock;                               // 用于计算每帧的时间
    sf::Time timeSinceLastUpdate = sf::Time::Zero; // 累积时间，用于固定时间步长更新

    std::cout << "Game loop starting..." << std::endl;

    // 主游戏循环
    while (m_window.isOpen())
    {
        sf::Time elapsedTime = clock.restart(); // 获取自上次调用 restart() 以来经过的时间
        timeSinceLastUpdate += elapsedTime;

        // 固定时间步长更新循环
        // 这确保了游戏的逻辑更新频率是固定的，不受渲染帧率波动的影响
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame; // 消耗一个时间步长

            processEvents();      // 处理窗口事件和用户输入
            update(TimePerFrame); // 以固定的 deltaTime 更新游戏逻辑
        }

        // 渲染，频率尽可能快（受垂直同步或 setFramerateLimit 限制）
        render();

        // 如果状态管理器为空了（比如所有状态都退出了），则关闭游戏
        if (m_stateManager.isEmpty())
        {
            std::cout << "StateManager is empty, closing window." << std::endl;
            m_window.close();
        }
    }
    std::cout << "Game loop ended." << std::endl;
}

void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    { // 处理所有待处理的事件
        // 首先处理窗口关闭事件，这是最高优先级的
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
            return; // 如果窗口关闭，不再处理其他事件
        }

        // 将其他事件传递给当前活动的状态进行处理
        m_stateManager.handleEvent(event);
    }
}

void Game::update(sf::Time deltaTime)
{
    // 更新当前活动的状态
    // deltaTime.asSeconds() 将 sf::Time 转换为浮点数秒
    m_stateManager.update(deltaTime.asSeconds());
}

void Game::render()
{
    m_window.clear(sf::Color(20, 20, 20)); // 用深灰色清屏，你可以选择其他颜色

    // 渲染当前活动的状态
    m_stateManager.render(m_window);

    m_window.display(); // 显示绘制的内容到屏幕
}

// 公共访问器方法
ResourceManager &Game::getResourceManager()
{
    return m_resourceManager;
}

StateManager &Game::getStateManager()
{
    return m_stateManager;
}

sf::RenderWindow &Game::getWindow()
{
    return m_window;
}
