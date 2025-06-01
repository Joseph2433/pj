#include "States/MenuState.h"
#include "Core/StateManager.h"
#include "Core/ResourceManager.h"
#include <iostream>

MenuState::MenuState(StateManager *stateManager)
    : GameState(stateManager), m_selectedIndex(0), m_useCustomFont(false)
{
}

void MenuState::init()
{
    // 尝试加载字体
    bool fontLoaded = false;
    // 系统字体
    if (m_font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        fontLoaded = true;
    }
    // 备用字体 Windows微软雅黑
    else if (m_font.loadFromFile("C:/Windows/Fonts/msyh.ttc"))
    {
        fontLoaded = true;
    }

    if (fontLoaded)
    {
        m_useCustomFont = true;
    }
    else
    {
        m_useCustomFont = false;
    }

    // 设置背景
    m_background.setSize(sf::Vector2f(800, 600));
    m_background.setFillColor(sf::Color(30, 30, 50));

    // 设置标题
    m_titleText.setString("Game Menu");
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

void MenuState::cleanup()
{
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

void MenuState::handleEvents(const sf::Event &event)
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
    // 这里可以添加菜单动画或其他更新逻辑
    // 例如：闪烁效果、背景动画等
}

void MenuState::render(sf::RenderWindow &window)
{
    // 渲染背景
    window.draw(m_background);

    // 渲染标题
    if (m_useCustomFont)
    {
        window.draw(m_titleText);
    }
    else
    {
        // 即使没有字体也尝试渲染，看看会发生什么
        window.draw(m_titleText);
    }

    // 渲染菜单项
    for (size_t i = 0; i < m_menuItems.size(); ++i)
    {
        const auto &item = m_menuItems[i];

        if (m_useCustomFont)
        {
            window.draw(item.text);
        }
        else
        {
            // 即使没有字体也尝试渲染
            window.draw(item.text);
        }
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
        // TODO: 切换到游戏状态
        // m_stateManager->changeState(std::make_unique<GamePlayState>(m_stateManager));
    }
    else if (action == "options")
    {
        // TODO: 切换到选项状态
        // m_stateManager->pushState(std::make_unique<OptionsState>(m_stateManager));
    }
    else if (action == "exit")
    {
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