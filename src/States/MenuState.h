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

    void init() override;
    void cleanup() override;
    void handleEvents(const sf::Event &event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

private:
    // 字体相关
    sf::Font m_font;
    bool m_useCustomFont = false; // 是否成功加载自定义字体
    // 使用系统字体

    // UI元素
    sf::RectangleShape m_background;
    sf::Text m_titleText;
    std::vector<MenuItem> m_menuItems;

    // 菜单状态
    int m_selectedIndex;

    // 私有方法
    void setupMenu();
    void updateSelection();
    void executeAction(const std::string &action);
    void moveUp();
    void moveDown();
    void select();
};