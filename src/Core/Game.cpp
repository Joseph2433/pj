#include "Game.h"
#include "States/MenuState.h"
#include "../Utils/Constants.h"
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
               WINDOW_TITLE,
               sf::Style::Default),
      m_resourceManager(),
      m_soundManager(),
      m_stateManager(this)
{
    m_window.setFramerateLimit(static_cast<unsigned int>(TARGET_FPS));
    std::cout << "Game object operated!" << std::endl;
    loadGlobalResources();
    m_stateManager.pushState(std::make_unique<MenuState>(&m_stateManager));
    if (m_stateManager.isEmpty())
    {
        std::cerr << "严重错误:没有初始状态被推入 StateManager!" << std::endl;
        m_window.close();
    }
}

// 加载全局资源
void Game::loadGlobalResources()
{
    std::cout << "Game:loading whole resource..." << std::endl;

    if (!m_resourceManager.hasFont(FONT_ID_PRIMARY))
    {
        if (!m_resourceManager.loadFont(FONT_ID_PRIMARY, FONT_PATH_ARIAL))
        {
            if (!m_resourceManager.loadFont(FONT_ID_PRIMARY, FONT_PATH_ARIAL))
            {
                std::cerr << "Game:严重 - 全局主要字体加载失败。" << std::endl;
            }
        }
    }
    if (!m_resourceManager.hasFont(FONT_ID_SECONDARY))
    {
        if (!m_resourceManager.loadFont(FONT_ID_SECONDARY, FONT_PATH_ARIAL))
        {
            std::cerr << "Game:全局次要字体加载失败。某些UI可能会使用主要字体作为备用。" << std::endl;
        }
    }

    // 预加载种子包
    std::cout << "Game: Pre-loading seed packet icons and shovel..." << std::endl;
    if (!m_resourceManager.hasTexture(SUNFLOWER_ICON_TEXTURE_KEY))
    {
        if (!m_resourceManager.loadTexture(SUNFLOWER_ICON_TEXTURE_KEY, "../../assets/images/sunflower.png"))
            std::cerr << "Game: Failed to load " << SUNFLOWER_ICON_TEXTURE_KEY << std::endl;
    }
    if (!m_resourceManager.hasTexture(PEASHOOTER_ICON_TEXTURE_KEY))
    {
        if (!m_resourceManager.loadTexture(PEASHOOTER_ICON_TEXTURE_KEY, "../../assets/images/peashooter.png"))
            std::cerr << "Game: Failed to load " << PEASHOOTER_ICON_TEXTURE_KEY << std::endl;
    }
    if (!m_resourceManager.hasTexture(WALLNUT_ICON_TEXTURE_KEY))
    {
        if (!m_resourceManager.loadTexture(WALLNUT_ICON_TEXTURE_KEY, "../../assets/images/wallnut.png"))
            std::cerr << "Game: Failed to load " << WALLNUT_ICON_TEXTURE_KEY << std::endl;
    }
    if (!m_resourceManager.hasTexture(ICE_PEASHOOTER_ICON_TEXTURE_KEY))
    {
        if (!m_resourceManager.loadTexture(ICE_PEASHOOTER_ICON_TEXTURE_KEY, "../../assets/images/ice_peashooter.png"))
            std::cerr << "Game: Failed to load " << ICE_PEASHOOTER_ICON_TEXTURE_KEY << std::endl;
    }
    if (!m_resourceManager.hasTexture(SHOVEL_TEXTURE_KEY))
    {
        if (!m_resourceManager.loadTexture(SHOVEL_TEXTURE_KEY, "../../assets/images/shovel.png"))
            std::cerr << "Game: Failed to load " << SHOVEL_TEXTURE_KEY << std::endl;
    }
    if (!m_resourceManager.hasTexture(SHOVEL_CURSOR_TEXTURE_KEY))
    {
        // Assuming same image for cursor for now, adjust path if different
        if (!m_resourceManager.loadTexture(SHOVEL_CURSOR_TEXTURE_KEY, "../../assets/images/shovel.png"))
            std::cerr << "Game: Failed to load " << SHOVEL_CURSOR_TEXTURE_KEY << std::endl;
    }

    // 背景音乐
    if (!m_soundManager.loadMusic(BGM_GAMEPLAY, "../../assets/audio/gameplay_music.mp3"))
    {
        std::cerr << "Game: Failed to load gameplay background music!" << std::endl;
    }
    std::cout << "Game:全局资源加载尝试完毕。" << std::endl;
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (m_window.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > TIME_PER_FRAME)
        {
            timeSinceLastUpdate -= TIME_PER_FRAME;

            processEvents();
            update(TIME_PER_FRAME);
        }

        render();

        if (m_stateManager.isEmpty())
        {
            m_window.close();
        }
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }
        m_stateManager.handleEvent(event);
    }
}

void Game::update(sf::Time deltaTime)
{
    m_stateManager.update(deltaTime.asSeconds());
}

void Game::render()
{
    m_window.clear(sf::Color(50, 50, 50));

    m_stateManager.render(m_window);

    m_window.display();
}

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
SoundManager &Game::getSoundManager()
{
    return m_soundManager;
}