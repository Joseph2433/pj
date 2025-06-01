#include "States/MenuState.h"
#include "Core/StateManager.h"
#include "Core/ResourceManager.h"
#include <iostream>

MenuState::MenuState(StateManager *stateManager)
    : GameState(stateManager), m_selectedIndex(0)
{
}

void MenuState::init()
{
    std::cout << "MenuState initialized" << std::endl;

    // 设置背景
    m_background.setSize(sf::Vector2f(800, 600));
    m_background.setFillColor(sf::Color(30, 30, 50));

    // 不使用自定义字体，直接使用系统默认字体
    // 设置标题 - 不设置字体，使用SFML默认字体
    m_titleText.setString("Game Menu");
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color::White);

    // 获取文本边界并居中
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setPosition((800 - titleBounds.width) / 2, 100);

    setupMenu();
}

void MenuState::cleanup()
{
    std::cout << "MenuState cleaned up" << std::endl;
    m_menuItems.clear();
}

void MenuState::setupMenu()
{
    m_menuItems.clear();

    // 创建菜单项 - 不使用字体参数
    m_menuItems.emplace_back("Start Game", "start");
    m_menuItems.emplace_back("Options", "options");
    m_menuItems.emplace_back("Exit", "exit");

    // 设置菜单项位置
    float startY = 300;
    float spacing = 80;

    for (size_t i = 0; i < m_menuItems.size(); ++i)
    {
        // 设置文本属性
        m_menuItems[i].text.setCharacterSize(36);
        m_menuItems[i].text.setFillColor(m_menuItems[i].normalColor);

        // 居中对齐
        sf::FloatRect textBounds = m_menuItems[i].text.getLocalBounds();
        m_menuItems[i].text.setPosition((800 - textBounds.width) / 2, startY + i * spacing);
    }

    // 选中第一个菜单项
    m_selectedIndex = 0;
    updateSelection();

    std::cout << "Menu setup completed with " << m_menuItems.size() << " items" << std::endl;
}

void MenuState::handleEvents(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        std::cout << "Key pressed: " << event.key.code << std::endl;

        switch (event.key.code)
        {
        case sf::Keyboard::Up:
            std::cout << "Moving up" << std::endl;
            moveUp();
            break;
        case sf::Keyboard::Down:
            std::cout << "Moving down" << std::endl;
            moveDown();
            break;
        case sf::Keyboard::Enter:
            std::cout << "Enter pressed" << std::endl;
            select();
            break;
        case sf::Keyboard::Escape:
            std::cout << "Escape pressed" << std::endl;
            executeAction("exit");
            break;
        default:
            break;
        }
    }
}

void MenuState::update(float deltaTime)
{
    // 这里可以添加菜单动画或其他更新逻辑
    // 例如：闪烁效果、背景动画等
}

void MenuState::render(sf::RenderWindow &window)
{
    // 渲染背景
    window.draw(m_background);

    // 渲染标题
    window.draw(m_titleText);

    // 渲染菜单项
    for (const auto &item : m_menuItems)
    {
        window.draw(item.text);
    }
}

void MenuState::updateSelection()
{
    std::cout << "Updating selection to index: " << m_selectedIndex << std::endl;

    for (size_t i = 0; i < m_menuItems.size(); ++i)
    {
        if (static_cast<int>(i) == m_selectedIndex)
        {
            m_menuItems[i].text.setFillColor(m_menuItems[i].selectedColor);
            m_menuItems[i].isSelected = true;
        }
        else
        {
            m_menuItems[i].text.setFillColor(m_menuItems[i].normalColor);
            m_menuItems[i].isSelected = false;
        }
    }
}

void MenuState::executeAction(const std::string &action)
{
    std::cout << "Executing action: " << action << std::endl;

    if (action == "start")
    {
        std::cout << "Starting game..." << std::endl;
        // TODO: 切换到游戏状态
        // m_stateManager->changeState(std::make_unique<GamePlayState>(m_stateManager));
    }
    else if (action == "options")
    {
        std::cout << "Opening options..." << std::endl;
        // TODO: 切换到选项状态
        // m_stateManager->pushState(std::make_unique<OptionsState>(m_stateManager));
    }
    else if (action == "exit")
    {
        std::cout << "Exiting game..." << std::endl;
        m_stateManager->clearStates();
    }
}

void MenuState::moveUp()
{
    m_selectedIndex--;
    if (m_selectedIndex < 0)
    {
        m_selectedIndex = static_cast<int>(m_menuItems.size()) - 1;
    }
    updateSelection();
}

void MenuState::moveDown()
{
    m_selectedIndex++;
    if (m_selectedIndex >= static_cast<int>(m_menuItems.size()))
    {
        m_selectedIndex = 0;
    }
    updateSelection();
}

void MenuState::select()
{
    if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_menuItems.size()))
    {
        executeAction(m_menuItems[m_selectedIndex].action);
    }
}