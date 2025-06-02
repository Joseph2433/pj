#include "States/MenuState.h"
#include "States/GamePlayState.h" // 确保包含这个头文件
#include "Core/StateManager.h"
#include <iostream>

MenuState::MenuState(StateManager *stateManager)
    : GameState(stateManager), m_selectedIndex(0), m_useCustomFont(false)
{
}

void MenuState::enter()
{
    std::cout << "Entering Menu State" << std::endl;

    // 尝试加载字体
    bool fontLoaded = false;

#ifdef _WIN32
    // Windows系统字体
    if (m_font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        fontLoaded = true;
    }
    else if (m_font.loadFromFile("C:/Windows/Fonts/msyh.ttc"))
    {
        fontLoaded = true;
    }
#else
    // Linux/Mac系统字体
    if (m_font.loadFromFile("/System/Library/Fonts/Arial.ttf") ||
        m_font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"))
    {
        fontLoaded = true;
    }
#endif

    m_useCustomFont = fontLoaded;

    // 设置背景
    m_background.setSize(sf::Vector2f(800, 600));
    m_background.setFillColor(sf::Color(30, 30, 50));

    // 设置标题
    m_titleText.setString("GAME MENU");
    if (m_useCustomFont)
    {
        m_titleText.setFont(m_font);
    }

    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color::White);

    // 获取文本边界并居中
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setPosition((800 - titleBounds.width) / 2, 100);

    setupMenu();
}

void MenuState::exit()
{
    std::cout << "Exiting Menu State" << std::endl;
    m_menuItems.clear();
}

void MenuState::setupMenu()
{
    m_menuItems.clear();

    // 创建菜单项
    m_menuItems.emplace_back("Start Game", "start");
    m_menuItems.emplace_back("Options", "options");
    m_menuItems.emplace_back("Exit", "exit");

    // 设置菜单项位置
    float startY = 300;
    float spacing = 80;

    for (size_t i = 0; i < m_menuItems.size(); ++i)
    {
        // 设置字体
        if (m_useCustomFont)
        {
            m_menuItems[i].text.setFont(m_font);
        }

        // 设置文本属性
        m_menuItems[i].text.setCharacterSize(36);
        m_menuItems[i].text.setFillColor(m_menuItems[i].normalColor);

        // 居中对齐
        sf::FloatRect textBounds = m_menuItems[i].text.getLocalBounds();
        float posX = (800 - textBounds.width) / 2;
        float posY = startY + i * spacing;
        m_menuItems[i].text.setPosition(posX, posY);
    }

    // 选中第一个菜单项
    m_selectedIndex = 0;
    updateSelection();
}

void MenuState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Up:
            moveUp();
            break;
        case sf::Keyboard::Down:
            moveDown();
            break;
        case sf::Keyboard::Enter:
            select();
            break;
        case sf::Keyboard::Escape:
            executeAction("exit");
            break;
        default:
            break;
        }
    }
}

void MenuState::update(float deltaTime)
{
    // 可以添加菜单动画或其他更新逻辑
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
    if (action == "start")
    {
        // 使用 this-> 来明确访问继承的成员
        this->m_stateManager->changeState(std::make_unique<GamePlayState>(this->m_stateManager));
    }
    else if (action == "options")
    {
        std::cout << "Options menu not implemented yet" << std::endl;
    }
    else if (action == "exit")
    {
        this->m_stateManager->clearStates();
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