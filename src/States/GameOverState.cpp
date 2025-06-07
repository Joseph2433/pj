#include "GameOverState.h"
#include "Core/StateManager.h"
#include "Core/Game.h" // 用于获取 ResourceManager 和窗口
#include "Core/ResourceManager.h"
#include "States/MenuState.h"     // 用于返回主菜单
#include "States/GamePlayState.h" // 用于重新开始
#include "../Utils/Constants.h"
#include <iostream>

GameOverState::GameOverState(StateManager *stateManager)
    : GameState(stateManager), m_fontLoaded(false)
{
}

void GameOverState::enter()
{
    std::cout << "Entering GameOver State" << std::endl;
    if (!m_stateManager || !m_stateManager->getGame())
    { /* ... error ... */
        return;
    }
    Game *game = m_stateManager->getGame();
    ResourceManager &resMan = game->getResourceManager();

    // 加载背景 (可以和MenuState背景一样，或者用新的)
    std::string bgTextureId = "GameOverBackground";
    std::string bgTexturePath = "../../assets/images/gameover_background.png";
    if (!resMan.hasTexture(bgTextureId))
    {
        if (!resMan.loadTexture(bgTextureId, bgTexturePath))
        {
            std::cerr << "GameOverState: Failed to load background texture: " << bgTexturePath << std::endl;
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

    // 加载字体 (推荐通过 ResourceManager)
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
        std::cerr << "GameOverState: Failed to load font." << std::endl;
    }

    m_gameOverText.setString("GAME OVER");
    if (m_fontLoaded)
        m_gameOverText.setFont(m_font);
    m_gameOverText.setCharacterSize(72);
    m_gameOverText.setFillColor(sf::Color::Red);
    m_gameOverText.setOutlineColor(sf::Color::Black);
    m_gameOverText.setOutlineThickness(2.f);
    sf::FloatRect textBounds = m_gameOverText.getLocalBounds();
    m_gameOverText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_gameOverText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 3.f);

    setupUI();
}

void GameOverState::setupUI()
{
    m_buttons.clear();
    const sf::Vector2f buttonSize(250.f, 60.f);
    const float buttonYStart = WINDOW_HEIGHT / 2.f + 50.f;
    const float buttonSpacing = 80.f;

    if (!m_fontLoaded)
    { // 如果字体未加载，按钮可能无法正确显示文本
        std::cerr << "GameOverState::setupUI - Font not loaded, buttons might not have text." << std::endl;
    }

    // 重试按钮
    m_buttons.emplace_back(Button(
        sf::Vector2f((WINDOW_WIDTH - buttonSize.x) / 2.f, buttonYStart),
        buttonSize, "Try Again", m_font // 确保 Button 构造能处理空字体
        ));
    m_buttons.back().setCallback([this]()
                                 { executeAction("retry"); });

    // 返回主菜单按钮
    m_buttons.emplace_back(Button(
        sf::Vector2f((WINDOW_WIDTH - buttonSize.x) / 2.f, buttonYStart + buttonSpacing),
        buttonSize, "Main Menu", m_font));
    m_buttons.back().setCallback([this]()
                                 { executeAction("menu"); });
}

void GameOverState::executeAction(const std::string &action)
{
    if (action == "retry")
    {
        m_stateManager->changeState(std::make_unique<GamePlayState>(m_stateManager));
    }
    else if (action == "menu")
    {
        m_stateManager->changeState(std::make_unique<MenuState>(m_stateManager));
    }
}

void GameOverState::exit()
{
    std::cout << "Exiting GameOver State" << std::endl;
    m_buttons.clear();
}

void GameOverState::handleEvent(const sf::Event &event)
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
        executeAction("menu"); // ESC 返回主菜单
    }
}

void GameOverState::update(float deltaTime)
{
    // 通常 Game Over 界面没有太多动态更新逻辑
}

void GameOverState::render(sf::RenderWindow &window)
{
    window.draw(m_backgroundSprite);
    window.draw(m_gameOverText);
    for (const auto &btn : m_buttons)
    {
        btn.render(window);
    }
}
