#include "States/MenuState.h"
#include "States/GamePlayState.h" // 用于切换状态
#include "Core/StateManager.h"
#include "Core/Game.h"            // <--- 确保包含
#include "Core/ResourceManager.h" // <--- 确保包含
#include "SFML/Graphics.hpp"      // 包含 RenderWindow 和其他图形类
#include "SFML/Window.hpp"        // 包含 Event
#include "../Utils/Constants.h"
#include <iostream>

MenuState::MenuState(StateManager *stateManager)
    : GameState(stateManager), m_useCustomFont(false), m_mousePosition(0.f, 0.f)
{
    // 构造函数保持简单
}

void MenuState::enter()
{
    std::cout << "Entering Menu State" << std::endl;

    // --- 获取必要的对象 ---
    if (!m_stateManager || !m_stateManager->getGame())
    {
        std::cerr << "MenuState::enter: Fatal error - StateManager or Game pointer is null!" << std::endl;
        return;
    }
    Game *game = m_stateManager->getGame();
    ResourceManager &resManager = game->getResourceManager(); // <--- 正确获取 ResourceManager
    // sf::RenderWindow& window = game->getWindow(); // 如果需要窗口特定信息，例如动态尺寸

    // --- 尝试加载字体 ---
    // (字体加载逻辑保持不变，但建议后续也通过 ResourceManager 管理)
    bool fontLoaded = false;
    if (m_font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    { // 或者使用 Constants.h 中的 FONT_PATH_ARIAL
        fontLoaded = true;
    }
    else if (m_font.loadFromFile("C:/Windows/Fonts/msyh.ttc"))
    {
        fontLoaded = true;
    }
    m_useCustomFont = fontLoaded;

    // --- 设置背景纹理 ---
    std::string backgroundTextureId = "MenuBackgroundTexture"; // ID可以自定义
    // !!! 确保此图片路径相对于你的可执行文件是正确的 !!!
    std::string backgroundTexturePath = "../../assets/images/menu_background.png"; // 假设这是你的菜单背景图片路径

    if (!resManager.hasTexture(backgroundTextureId))
    {
        if (!resManager.loadTexture(backgroundTextureId, backgroundTexturePath))
        {
            std::cerr << "MenuState::enter: Failed to load background texture: " << backgroundTexturePath << std::endl;
            // 如果加载失败，m_BackgroundSpite 将使用 ResourceManager 中的默认纹理
        }
    }
    m_BackgroundSpite.setTexture(resManager.getTexture(backgroundTextureId));

    // (可选) 缩放背景以适应窗口
    const sf::Texture *tex = m_BackgroundSpite.getTexture();
    if (tex && tex->getSize().x > 0 && tex->getSize().y > 0)
    {
        sf::Vector2u textureSize = tex->getSize();
        m_BackgroundSpite.setScale(
            static_cast<float>(WINDOW_WIDTH) / textureSize.x,
            static_cast<float>(WINDOW_HEIGHT) / textureSize.y);
    }
    m_BackgroundSpite.setPosition(0.f, 0.f);
    // --- 背景纹理设置完毕 ---

    // --- 设置标题 ---
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

    setupButtons(); // 初始化按钮
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

    // 假设 Button 构造函数能够处理 m_font (即使它未成功加载)
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
    // 当前无更新逻辑
}

void MenuState::render(sf::RenderWindow &window)
{
    window.draw(m_BackgroundSpite); // <--- 修正：绘制 Sprite 而不是未定义的 m_background

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
        // 关闭游戏窗口或清空状态栈
        if (m_stateManager && m_stateManager->getGame())
        {
            m_stateManager->getGame()->getWindow().close();
        }
        // 或者 m_stateManager->clearStates(); 效果类似，但窗口会因状态栈空而关闭
    }
}
