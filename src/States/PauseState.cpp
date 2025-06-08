#include "PauseState.h"
#include "Core/StateManager.h"
#include "../Utils/SoundManager.h"
#include "Core/Game.h"
#include "Core/ResourceManager.h"
#include "States/MenuState.h"
#include "States/GamePlayState.h"
#include "../Utils/Constants.h"
#include <iostream>

PauseState::PauseState(StateManager *stateManager)
    : GameState(stateManager), m_fontLoaded(false)
{
    std::cout << "PauseState constructing..." << std::endl;
}

void PauseState::enter()
{
    std::cout << "Entering Pause State" << std::endl;
    if (!m_stateManager || !m_stateManager->getGame())
    {
        return;
    }
    Game *game = m_stateManager->getGame();
    ResourceManager &resMan = game->getResourceManager();
    sf::RenderWindow &window = game->getWindow();

    // 1. 设置半透明背景遮罩
    m_backgroundOverlay.setSize(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
    m_backgroundOverlay.setFillColor(sf::Color(0, 0, 0, 150));
    m_backgroundOverlay.setPosition(0, 0);

    // 2. 加载字体
    if (resMan.hasFont(FONT_ID_PRIMARY))
    {
        m_font = resMan.getFont(FONT_ID_PRIMARY);
        m_fontLoaded = true;
    }
    else if (m_font.loadFromFile(FONT_PATH_ARIAL))
    {
        m_fontLoaded = true;
        std::cerr << "PauseState: Primary font not found, loaded fallback Arial." << std::endl;
    }
    else
    {
        std::cerr << "PauseState: Failed to load any font for pause text." << std::endl;
    }

    // 3. 设置 "Paused" 文本
    m_pauseText.setString("GAME PAUSED");
    if (m_fontLoaded)
        m_pauseText.setFont(m_font);
    m_pauseText.setCharacterSize(60);
    m_pauseText.setFillColor(sf::Color::White);
    m_pauseText.setOutlineColor(sf::Color::Black);
    m_pauseText.setOutlineThickness(2.f);
    sf::FloatRect textBounds = m_pauseText.getLocalBounds();
    m_pauseText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    m_pauseText.setPosition(window.getSize().x / 2.f, window.getSize().y / 3.f);

    // 4. 设置按钮
    setupUI();
    std::cout << "PauseState entered." << std::endl;
    if (!m_stateManager || !m_stateManager->getGame())
    {
        return;
    }
    SoundManager &soundMan = m_stateManager->getGame()->getSoundManager();
    if (soundMan.getMusicStatus() == sf::SoundSource::Playing)
    {
        // 只暂停正在播放的音乐
        soundMan.pauseMusic();
        std::cout << "PauseState: Music paused." << std::endl;
    }
}

void PauseState::setupUI()
{
    m_buttons.clear();
    const sf::Vector2f buttonSize(280.f, 50.f);
    const float initialButtonY = m_pauseText.getPosition().y + m_pauseText.getGlobalBounds().height + 70.f;
    const float buttonSpacing = 70.f;
    float currentButtonY = initialButtonY;

    if (!m_fontLoaded && m_buttons.empty())
    {
        std::cerr << "PauseState::setupUI - Font not loaded, cannot create buttons with text." << std::endl;
    }

    // Resume Button
    m_buttons.emplace_back(Button(
        sf::Vector2f((WINDOW_WIDTH - buttonSize.x) / 2.f, currentButtonY),
        buttonSize, "Resume Game", m_font));
    m_buttons.back().setCallback([this]()
                                 { executeAction("resume"); });
    currentButtonY += buttonSpacing;

    // Restart Button
    m_buttons.emplace_back(Button(
        sf::Vector2f((WINDOW_WIDTH - buttonSize.x) / 2.f, currentButtonY),
        buttonSize, "Restart", m_font));
    m_buttons.back().setCallback([this]()
                                 { executeAction("restart"); });
    currentButtonY += buttonSpacing;

    // Main Menu Button
    m_buttons.emplace_back(Button(
        sf::Vector2f((WINDOW_WIDTH - buttonSize.x) / 2.f, currentButtonY),
        buttonSize, "Main Menu", m_font));
    m_buttons.back().setCallback([this]()
                                 { executeAction("menu"); });
}

void PauseState::executeAction(const std::string &action)
{
    if (action == "resume")
    {
        std::cout << "PauseState: Action 'resume'. Popping PauseState." << std::endl;
        m_stateManager->popState();
    }
    else if (action == "restart")
    {
        std::cout << "PauseState: Action 'restart'. Popping PauseState first." << std::endl;
        m_stateManager->popState();

        std::cout << "PauseState: After popping PauseState, stack empty? "
                  << (m_stateManager->isEmpty() ? "Yes" : "No") << std::endl;

        if (!m_stateManager->isEmpty())
        {
            GameState *currentState = m_stateManager->getCurrentState();
            if (currentState)
            {
                std::cout << "PauseState: Current top state pointer is valid." << std::endl;
                GamePlayState *gameplayState = dynamic_cast<GamePlayState *>(currentState);
                std::cout << "PauseState: gameplayState pointer after dynamic_cast: " << gameplayState << std::endl;

                if (gameplayState)
                {
                    std::cout << "PauseState: Successfully cast to GamePlayState. Requesting resetLevel." << std::endl;
                    gameplayState->resetLevel();
                }
                else
                {
                    std::cerr << "PauseState Error: dynamic_cast to GamePlayState FAILED! Current state is not GamePlayState?" << std::endl;
                    std::cerr << "PauseState: Fallback - changing to a new GamePlayState." << std::endl;
                    m_stateManager->changeState(std::make_unique<GamePlayState>(m_stateManager));
                }
            }
            else
            {
                std::cerr << "PauseState Error: getCurrentState() returned nullptr after popping PauseState." << std::endl;
                std::cerr << "PauseState: Fallback - pushing a new GamePlayState onto (presumably) empty stack." << std::endl;
                m_stateManager->pushState(std::make_unique<GamePlayState>(m_stateManager));
            }
        }
        else
        {
            std::cerr << "PauseState Error: Stack became empty immediately after popping PauseState. This should not happen." << std::endl;
            std::cerr << "PauseState: Starting a new GamePlayState as a recovery measure." << std::endl;
            m_stateManager->pushState(std::make_unique<GamePlayState>(m_stateManager));
        }
    }
    else if (action == "menu")
    {
        std::cout << "PauseState: Action 'menu'. Clearing all states and pushing MenuState." << std::endl;
        m_stateManager->clearStates();
        m_stateManager->pushState(std::make_unique<MenuState>(m_stateManager));
    }
}
void PauseState::exit()
{
    std::cout << "Exiting Pause State" << std::endl;
    m_buttons.clear();
    if (!m_stateManager || !m_stateManager->getGame())
    {
        return;
    }
    SoundManager &soundMan = m_stateManager->getGame()->getSoundManager();
    if (soundMan.getMusicStatus() == sf::SoundSource::Paused)
    {
        soundMan.resumeMusic();
        std::cout << "PauseState: Music resumed." << std::endl;
    }
}

void PauseState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::P)
        {
            // ESC 或 P 都可以取消暂停
            executeAction("resume");
            return;
        }
    }

    if (event.type == sf::Event::MouseMoved)
    {
        sf::RenderWindow &window = m_stateManager->getGame()->getWindow();
        m_mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), window.getView());
        for (auto &btn : m_buttons)
            btn.handleMouseMove(m_mousePosition);
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::RenderWindow &window = m_stateManager->getGame()->getWindow();
        sf::Vector2f clickPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), window.getView());
        for (auto &btn : m_buttons)
            btn.handleMouseClick(clickPos);
    }
}

void PauseState::update(float deltaTime)
{
}

void PauseState::render(sf::RenderWindow &window)
{

    window.draw(m_backgroundOverlay);

    window.draw(m_pauseText);

    for (const auto &btn : m_buttons)
    {
        btn.render(window);
    }
}
