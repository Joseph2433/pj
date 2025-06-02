#include "States/MenuState.h"
#include "States/GamePlayState.h"
#include "Core/StateManager.h"
#include "../Utils/Constants.h"
#include <iostream>

MenuState::MenuState(StateManager *stateManager)
    : GameState(stateManager), m_useCustomFont(false), m_mousePosition(0.f, 0.f)
{
}

void MenuState::enter()
{
    std::cout << "Entering Menu State" << std::endl;

    // 尝试加载字体
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
    m_background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    m_background.setFillColor(sf::Color(30, 30, 50));
    m_background.setPosition(0, 0);

    // 设置标题
    m_titleText.setString("GAME MENU");
    if (m_useCustomFont)
    {
        m_titleText.setFont(m_font);
    }
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color::White);

    // 标题居中
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setPosition(
        (WINDOW_WIDTH - titleBounds.width) / 2.f,
        100.f);

    setupButtons(); // 初始化按钮
}

void MenuState::exit()
{
    std::cout << "Exiting Menu State" << std::endl;
    m_buttons.clear(); // 清理按钮容器
}

void MenuState::setupButtons()
{
    m_buttons.clear(); // 确保容器清空

    // 按钮尺寸和垂直间距（与原菜单布局一致）
    const sf::Vector2f buttonSize(200.f, 50.f);
    const float startY = 300.f;
    const float spacing = 80.f;

    // 创建三个按钮并设置位置
    m_buttons.emplace_back(Button(
        sf::Vector2f(
            (WINDOW_WIDTH - buttonSize.x) / 2.f,
            startY + 0 * spacing),
        buttonSize,
        "Start Game",
        m_font));

    m_buttons.emplace_back(Button(
        sf::Vector2f(
            (WINDOW_WIDTH - buttonSize.x) / 2.f,
            startY + 1 * spacing),
        buttonSize,
        "Options",
        m_font));

    m_buttons.emplace_back(Button(
        sf::Vector2f(
            (WINDOW_WIDTH - buttonSize.x) / 2.f,
            startY + 2 * spacing),
        buttonSize,
        "Exit",
        m_font));

    // 设置按钮点击回调（绑定到原有逻辑）
    m_buttons[0].setCallback([this]()
                             { executeAction("start"); });
    m_buttons[1].setCallback([this]()
                             { executeAction("options"); });
    m_buttons[2].setCallback([this]()
                             { executeAction("exit"); });
}

void MenuState::handleEvent(const sf::Event &event)
{
    // 处理鼠标移动事件（更新悬停状态）
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

    // 处理鼠标左键点击事件（触发按钮回调）
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        for (auto &btn : m_buttons)
        {
            btn.handleMouseClick(m_mousePosition);
        }
    }

    // 保留 ESC 键退出功能（可选，如需完全移除键盘可注释以下代码）
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape)
    {
        executeAction("exit");
    }
}

void MenuState::update(float deltaTime)
{
    // 可添加按钮动画逻辑（如淡入淡出、抖动等）
    // 示例：if (m_buttons[0].isHovered()) { ... }
}

void MenuState::render(sf::RenderWindow &window)
{
    window.draw(m_background);
    window.draw(m_titleText);

    // 渲染所有按钮
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
        m_stateManager->clearStates(); // 退出游戏（根据状态管理逻辑）
    }
}