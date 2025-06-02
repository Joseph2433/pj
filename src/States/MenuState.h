#pragma once
#include "Core/GameState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../UI/Button.h" // 添加按钮头文件

class StateManager;

// 移除原有 MenuItem 结构体（改用 Button 类）
// struct MenuItem { ... } 不再需要

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
    // 字体相关
    sf::Font m_font;
    bool m_useCustomFont = false;

    // UI元素
    sf::RectangleShape m_background;
    sf::Text m_titleText;
    std::vector<Button> m_buttons; // 使用按钮容器
    sf::Vector2f m_mousePosition;  // 鼠标位置

    // 私有方法
    sf::Text setupText(const std::string &text, unsigned int size, sf::Color color, float yOffset) const;
    void setupButtons();                           // 替换原有 setupMenu
    void executeAction(const std::string &action); // 保留原有逻辑
};