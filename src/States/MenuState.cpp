#include "States/MenuState.h"
#include "States/GamePlayState.h"
#include "Core/StateManager.h"
#include "Core/Game.h"
#include "Core/ResourceManager.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "../Utils/Constants.h"
#include <iostream>

MenuState::MenuState(StateManager *stateManager)
    : GameState(stateManager), m_useCustomFont(false), m_mousePosition(0.f, 0.f)
{
}

void MenuState::enter()
{
    std::cout << "Entering Menu State" << std::endl;
    if (!m_stateManager || !m_stateManager->getGame())
    {
        std::cerr << "MenuState::enter: Fatal error - StateManager or Game pointer is null!" << std::endl;
        return;
    }
    Game *game = m_stateManager->getGame();
    ResourceManager &resManager = game->getResourceManager();

    // 加载字体
    bool fontLoaded = false;
    if (m_font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        fontLoaded = true;
    }
    else if (m_font.loadFromFile("C:/Windows/Fonts/msyh.ttc"))
    {
        fontLoaded = true;
    }
    m_useCustomFont = fontLoaded;

    // 设置背景
    std::string backgroundTextureId = "MenuBackgroundTexture";
    std::string backgroundTexturePath = "../../assets/images/menu_background.png";

    if (!resManager.hasTexture(backgroundTextureId))
    {
        if (!resManager.loadTexture(backgroundTextureId, backgroundTexturePath))
        {
            std::cerr << "MenuState::enter: Failed to load background texture: " << backgroundTexturePath << std::endl;
        }
    }
    m_BackgroundSpite.setTexture(resManager.getTexture(backgroundTextureId));

    const sf::Texture *tex = m_BackgroundSpite.getTexture();
    if (tex && tex->getSize().x > 0 && tex->getSize().y > 0)
    {
        sf::Vector2u textureSize = tex->getSize();
        m_BackgroundSpite.setScale(
            static_cast<float>(WINDOW_WIDTH) / textureSize.x,
            static_cast<float>(WINDOW_HEIGHT) / textureSize.y);
    }
    m_BackgroundSpite.setPosition(0.f, 0.f);

    // 设置标题
    m_titleText.setString("Defend Mixue");
    if (m_useCustomFont)
    {
        m_titleText.setFont(m_font);
    }
    m_titleText.setCharacterSize(60);
    m_titleText.setFillColor(sf::Color(0, 0, 0));
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setPosition(
        (WINDOW_WIDTH - titleBounds.width) / 2.f,
        150.f);

    setupButtons();
}

void MenuState::exit()
{
    std::cout << "Exiting Menu State" << std::endl;
    m_buttons.clear();
}

void MenuState::setupButtons()
{
    m_buttons.clear();
    const sf::Vector2f buttonSize(200.f, 50.f);
    const float startY = 300.f;
    const float spacing = 80.f;

    m_buttons.emplace_back(Button(
        sf::Vector2f((WINDOW_WIDTH - buttonSize.x) / 2.f, startY + 0 * spacing),
        buttonSize, "Start Game", m_font));
    m_buttons.emplace_back(Button(
        sf::Vector2f((WINDOW_WIDTH - buttonSize.x) / 2.f, startY + 1 * spacing),
        buttonSize, "Options", m_font));
    m_buttons.emplace_back(Button(
        sf::Vector2f((WINDOW_WIDTH - buttonSize.x) / 2.f, startY + 2 * spacing),
        buttonSize, "Exit", m_font));

    m_buttons[0].setCallback([this]()
                             { executeAction("start"); });
    m_buttons[1].setCallback([this]()
                             { executeAction("options"); });
    m_buttons[2].setCallback([this]()
                             { executeAction("exit"); });
}

void MenuState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::MouseMoved)
    {
        m_mousePosition = sf::Vector2f(
            static_cast<float>(event.mouseMove.x),
            static_cast<float>(event.mouseMove.y));
        for (auto &btn : m_buttons)
        {
            btn.handleMouseMove(m_mousePosition);
        }
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        for (auto &btn : m_buttons)
        {
            btn.handleMouseClick(m_mousePosition);
        }
    }

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape)
    {
        executeAction("exit");
    }
}

void MenuState::update(float deltaTime)
{
}

void MenuState::render(sf::RenderWindow &window)
{
    window.draw(m_BackgroundSpite);

    window.draw(m_titleText);
    for (auto &btn : m_buttons)
    {
        btn.render(window);
    }
}

void MenuState::executeAction(const std::string &action)
{
    if (action == "start")
    {
        m_stateManager->changeState(
            std::make_unique<GamePlayState>(m_stateManager));
    }
    else if (action == "options")
    {
        std::cout << "Options menu not implemented yet" << std::endl;
    }
    else if (action == "exit")
    {
        if (m_stateManager && m_stateManager->getGame())
        {
            m_stateManager->getGame()->getWindow().close();
        }
    }
}
