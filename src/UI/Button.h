#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

// 简单的按钮类，用于菜单和UI交互
class Button
{
public:
    Button(const sf::Vector2f &position, const sf::Vector2f &size,
           const std::string &text, const sf::Font &font);

    // 渲染按钮
    void render(sf::RenderWindow &window);

    // 检查是否被点击
    bool isClicked(const sf::Vector2f &mousePos) const;

    // 设置按钮状态
    void setEnabled(bool enabled) { m_enabled = enabled; }
    bool isEnabled() const { return m_enabled; }

    // 设置按钮颜色
    void setButtonColor(const sf::Color &color) { m_buttonShape.setFillColor(color); }
    void setTextColor(const sf::Color &color) { m_text.setFillColor(color); }

private:
    sf::RectangleShape m_buttonShape; // 按钮的矩形形状
    sf::Text m_text;                  // 按钮上的文字
    bool m_enabled;                   // 是否启用状态

    // 更新按钮外观
    void updateAppearance();
};

#endif