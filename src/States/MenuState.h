#pragma once
#include "Core/GameState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class StateManager;

struct MenuItem
{
    sf::Text text;
    std::string action;
    sf::Color normalColor = sf::Color::White;
    sf::Color selectedColor = sf::Color::Yellow;
    bool isSelected = false;

    MenuItem(const std::string &str, const std::string &act)
        : action(act), isSelected(false)
    {
        text.setString(str);
    }
};

class MenuState : public GameState
{
public:
    MenuState(StateManager *stateManager);

    // 实现基类的纯虚函数
    void enter() override;
    void exit() override;
    void handleEvent(const sf::Event &event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

private:
    // 注意：m_stateManager 已经在基类中定义，不需要重复声明

    // 字体相关
    sf::Font m_font;
    bool m_useCustomFont = false;

    // UI元素
    sf::RectangleShape m_background;
    sf::Text m_titleText;
    std::vector<MenuItem> m_menuItems;

    // 菜单状态
    int m_selectedIndex = 0; // 初始化为0

    // 私有方法
    void setupMenu();
    void updateSelection();
    void executeAction(const std::string &action);
    void moveUp();
    void moveDown();
    void select();
};