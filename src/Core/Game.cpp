
#include "Game.h"
#include "../Utils/Constants.h"
#include "../States/MenuState.h"

Game *Game::s_instance = nullptr;

Game::Game()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE), m_isRunning(false)
{
    s_instance = this;
}

Game::~Game()
{
    cleanup();
    s_instance = nullptr;
}

Game &Game::getInstance()
{
    return *s_instance;
}

void Game::initialize()
{
    // 设置窗口属性
    m_window.setFramerateLimit(static_cast<unsigned int>(FRAME_RATE));

    // 初始化管理器
    m_resourceManager = std::make_unique<ResourceManager>();
    m_stateManager = std::make_unique<StateManager>();
    m_grid = std::make_unique<Grid>();

    // 初始化网格
    m_grid->initialize();

    // 设置初始状态
    m_stateManager->pushState(std::make_unique<MenuState>(m_stateManager.get()));

    m_isRunning = true;
}

void Game::run()
{
    initialize();

    while (m_isRunning && m_window.isOpen())
    {
        float deltaTime = m_clock.restart().asSeconds();

        handleEvents();
        update(deltaTime);
        render();
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
            m_isRunning = false;
            m_window.close();
        }

        // 将事件传递给当前状态
        if (m_stateManager->getCurrentState())
        {
            m_stateManager->getCurrentState()->handleEvent(event);
        }
    }
}

void Game::update(float deltaTime)
{
    // 更新状态管理器
    m_stateManager->update(deltaTime);

    // 如果没有状态了，退出游戏
    if (!m_stateManager->getCurrentState())
    {
        m_isRunning = false;
    }
}

void Game::render()
{
    // 使用黑色背景（让状态自己处理背景）
    m_window.clear(sf::Color::Black);

    // 不再渲染全局网格

    // 渲染当前状态（状态自己处理背景和网格）
    if (m_stateManager->getCurrentState())
    {
        m_stateManager->getCurrentState()->render(m_window);
    }

    m_window.display();
}

void Game::cleanup()
{
    m_stateManager.reset();
    m_resourceManager.reset();
    m_grid.reset();
}

StateManager &Game::getStateManager()
{
    return *m_stateManager;
}

ResourceManager &Game::getResourceManager()
{
    return *m_resourceManager;
}

Grid &Game::getGrid()
{
    return *m_grid;
}

sf::RenderWindow &Game::getWindow()
{
    return m_window;
}