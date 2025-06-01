#include "Core/Game.h"
#include "Core/StateManager.h"
#include "Core/ResourceManager.h"
#include "States/MenuState.h"
#include <iostream>

Game::Game() : m_isRunning(false)
{
    std::cout << "Game constructor called" << std::endl;
    std::cout.flush();
}

Game::~Game()
{
    std::cout << "Game destructor called" << std::endl;
    std::cout.flush();
    cleanup();
}

void Game::init()
{
    std::cout << "Game::init() - Starting initialization..." << std::endl;
    std::cout.flush();

    try
    {
        // 创建窗口
        std::cout << "Creating window..." << std::endl;
        std::cout.flush();

        m_window.create(sf::VideoMode(800, 600), "Game Project");
        m_window.setFramerateLimit(60);

        std::cout << "Window created successfully" << std::endl;
        std::cout.flush();

        // 初始化状态管理器
        std::cout << "Creating StateManager..." << std::endl;
        std::cout.flush();

        m_stateManager = std::make_unique<StateManager>();

        std::cout << "StateManager created successfully" << std::endl;
        std::cout.flush();

        // 初始化资源管理器（加载基础资源）
        std::cout << "Getting ResourceManager instance..." << std::endl;
        std::cout.flush();

        ResourceManager &rm = ResourceManager::getInstance();

        std::cout << "ResourceManager instance obtained" << std::endl;
        std::cout.flush();

        // 设置初始状态为菜单状态
        std::cout << "Creating MenuState..." << std::endl;
        std::cout.flush();

        m_stateManager->pushState(std::make_unique<MenuState>(m_stateManager.get()));

        std::cout << "MenuState created and pushed" << std::endl;
        std::cout.flush();

        m_isRunning = true;
        std::cout << "Game initialized successfully" << std::endl;
        std::cout.flush();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in Game::init(): " << e.what() << std::endl;
        std::cerr.flush();
        m_isRunning = false;
        throw;
    }
    catch (...)
    {
        std::cerr << "Unknown exception in Game::init()" << std::endl;
        std::cerr.flush();
        m_isRunning = false;
        throw;
    }
}

void Game::run()
{
    std::cout << "Game::run() - Starting..." << std::endl;
    std::cout.flush();

    try
    {
        init();

        std::cout << "Entering main game loop..." << std::endl;
        std::cout.flush();

        while (m_isRunning && m_window.isOpen())
        {
            float deltaTime = m_clock.restart().asSeconds();

            handleEvents();
            update(deltaTime);
            render();

            // 如果状态管理器为空，退出游戏
            if (m_stateManager->isEmpty())
            {
                std::cout << "StateManager is empty, exiting..." << std::endl;
                std::cout.flush();
                m_isRunning = false;
            }
        }

        std::cout << "Main game loop ended" << std::endl;
        std::cout.flush();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in Game::run(): " << e.what() << std::endl;
        std::cerr.flush();
    }
    catch (...)
    {
        std::cerr << "Unknown exception in Game::run()" << std::endl;
        std::cerr.flush();
    }

    cleanup();
}

void Game::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            std::cout << "Window close event received" << std::endl;
            std::cout.flush();
            m_isRunning = false;
        }

        // 将事件传递给状态管理器
        if (m_stateManager)
        {
            m_stateManager->handleEvents(event);
        }
    }
}

void Game::update(float deltaTime)
{
    if (m_stateManager)
    {
        m_stateManager->update(deltaTime);
    }
}

void Game::render()
{
    m_window.clear(sf::Color::Black);

    // 让状态管理器渲染当前状态
    if (m_stateManager)
    {
        m_stateManager->render(m_window);
    }

    m_window.display();
}

void Game::cleanup()
{
    std::cout << "Game::cleanup() - Starting cleanup..." << std::endl;
    std::cout.flush();

    if (m_stateManager)
    {
        m_stateManager->clearStates();
        m_stateManager.reset();
    }

    ResourceManager::getInstance().clear();

    if (m_window.isOpen())
    {
        m_window.close();
    }

    std::cout << "Game cleaned up" << std::endl;
    std::cout.flush();
}