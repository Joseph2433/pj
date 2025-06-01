#pragma once
#include "Core/GameState.h"
#include <SFML/Graphics.hpp>
#include <vector>

class MenuState : public GameState
{
public:
    MenuState(StateManager *stateManager);
    virtual ~MenuState() = default;

    // 继承的函数
    void init() override;
    void cleanup() override;
    void handleEvents(const sf::Event &event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

private:
    struct MenuItem
    {
        sf::Text text;
        std::string action;
        sf::Color normalColor;
        sf::Color selectedColor;
        bool isSelected;

        // 简化构造函数，不需要字体参数
        MenuItem(const std::string &str, const std::string &act)
            : action(act), normalColor(sf::Color::White), selectedColor(sf::Color::Yellow), isSelected(false)
        {
            text.setString(str);
            // 不设置字体，使用SFML默认字体
            text.setCharacterSize(30);
            text.setFillColor(normalColor);
        }
    };

    // 菜单项
    std::vector<MenuItem> m_menuItems;
    int m_selectedIndex;

    // UI元素
    sf::Text m_titleText;
    sf::RectangleShape m_background;

    // 方法
    void setupMenu();
    void updateSelection();
    void executeAction(const std::string &action);
    void moveUp();
    void moveDown();
    void select();
};