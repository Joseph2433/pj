#include "VictoryState.h"
#include "Core/StateManager.h"
#include "Core/Game.h"
#include "Core/ResourceManager.h"
#include "States/MenuState.h"
#include "States/GamePlayState.h"
#include "../Utils/Constants.h"
#include <iostream>

VictoryState::VictoryState(StateManager *stateManager)
    : GameState(stateManager), m_fontLoaded(false)
{
}

void VictoryState::enter()
{
    std::cout << "Entering Victory State" << std::endl;
    if (!m_stateManager || !m_stateManager->getGame())
    {
        return;
    }
    Game *game = m_stateManager->getGame();
    ResourceManager &resMan = game->getResourceManager();

    // 加载背景
    std::string bgTextureId = "VictoryBackground";
    std::string bgTexturePath = "../../assets/images/victory_background.png";
    if (!resMan.hasTexture(bgTextureId))
    {
        if (!resMan.loadTexture(bgTextureId, bgTexturePath))
        {
            std::cerr << "VictoryState: Failed to load background texture: " << bgTexturePath << std::endl;
        }
    }
    m_backgroundSprite.setTexture(resMan.getTexture(bgTextureId));
    const sf::Texture *tex = m_backgroundSprite.getTexture();
    if (tex && tex->getSize().x > 0 && tex->getSize().y > 0)
    {
        m_backgroundSprite.setScale(
            static_cast<float>(WINDOW_WIDTH) / tex->getSize().x,
            static_cast<float>(WINDOW_HEIGHT) / tex->getSize().y);
    }
    m_backgroundSprite.setPosition(0, 0);

    if (resMan.hasFont(FONT_ID_PRIMARY))
    {
        m_font = resMan.getFont(FONT_ID_PRIMARY);
        m_fontLoaded = true;
    }
    else if (m_font.loadFromFile(FONT_PATH_ARIAL))
    {
        m_fontLoaded = true;
    }
    else
    {
        std::cerr << "VictoryState: Failed to load font." << std::endl;
    }

    m_victoryText.setString("VICTORY!");
    if (m_fontLoaded)
        m_victoryText.setFont(m_font);
    m_victoryText.setCharacterSize(80);
    m_victoryText.setFillColor(sf::Color::Yellow);
    m_victoryText.setOutlineColor(sf::Color(150, 100, 0));
    m_victoryText.setOutlineThickness(3.f);
    sf::FloatRect textBounds = m_victoryText.getLocalBounds();
    m_victoryText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_victoryText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 3.f - 40.f);

    m_congratsText.setString("You survived all waves!");
    if (m_fontLoaded)
        m_congratsText.setFont(m_font);
    m_congratsText.setCharacterSize(36);
    m_congratsText.setFillColor(sf::Color::White);
    textBounds = m_congratsText.getLocalBounds();
    m_congratsText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_congratsText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 3.f + 50.f);

    setupUI();
}

void VictoryState::setupUI()
{
    m_buttons.clear();
    const sf::Vector2f buttonSize(280.f, 60.f);
    const float buttonYStart = WINDOW_HEIGHT / 2.f + 80.f;
    const float buttonSpacing = 80.f;

    // 重新开始
    m_buttons.emplace_back(Button(
        sf::Vector2f((WINDOW_WIDTH - buttonSize.x) / 2.f, buttonYStart),
        buttonSize, "Play Again", m_font));
    m_buttons.back().setCallback([this]()
                                 { executeAction("play_again"); });

    // 返回主菜单
    m_buttons.emplace_back(Button(
        sf::Vector2f((WINDOW_WIDTH - buttonSize.x) / 2.f, buttonYStart + buttonSpacing),
        buttonSize, "Main Menu", m_font));
    m_buttons.back().setCallback([this]()
                                 { executeAction("menu"); });
}

void VictoryState::executeAction(const std::string &action)
{
    if (action == "play_again")
    {
        m_stateManager->changeState(std::make_unique<GamePlayState>(m_stateManager));
    }
    else if (action == "menu")
    {
        m_stateManager->changeState(std::make_unique<MenuState>(m_stateManager));
    }
}

void VictoryState::exit()
{
    std::cout << "Exiting Victory State" << std::endl;
    m_buttons.clear();
}
void VictoryState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::MouseMoved)
    {
        m_mousePosition = sf::Vector2f(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
        for (auto &btn : m_buttons)
            btn.handleMouseMove(m_mousePosition);
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        for (auto &btn : m_buttons)
            btn.handleMouseClick(m_mousePosition);
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        executeAction("menu");
    }
}
void VictoryState::update(float deltaTime) {}
void VictoryState::render(sf::RenderWindow &window)
{
    window.draw(m_backgroundSprite);
    window.draw(m_victoryText);
    window.draw(m_congratsText);
    for (const auto &btn : m_buttons)
    {
        btn.render(window);
    }
}
