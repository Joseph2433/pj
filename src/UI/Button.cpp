#include "Button.h"

Button::Button(const sf::Vector2f &position, const sf::Vector2f &size,
               const std::string &text, const sf::Font &font)
    : m_enabled(true)
{
    // 设置按钮矩形
    m_buttonShape.setPosition(position);
    m_buttonShape.setSize(size);
    m_buttonShape.setFillColor(sf::Color(100, 150, 100)); // 深绿色
    m_buttonShape.setOutlineThickness(2);
    m_buttonShape.setOutlineColor(sf::Color(50, 100, 50)); // 更深的绿色边框

    // 设置按钮文字
    m_text.setFont(font);
    m_text.setString(text);
    m_text.setCharacterSize(24);
    m_text.setFillColor(sf::Color::White);

    // 计算文字位置，使其在按钮中央
    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setPosition(
        position.x + (size.x - textBounds.width) / 2.0f,
        position.y + (size.y - textBounds.height) / 2.0f - textBounds.top);
}

void Button::render(sf::RenderWindow &window)
{
    updateAppearance();
    window.draw(m_buttonShape);
    window.draw(m_text);
}

bool Button::isClicked(const sf::Vector2f &mousePos) const
{
    return m_enabled && m_buttonShape.getGlobalBounds().contains(mousePos);
}

void Button::updateAppearance()
{
    if (m_enabled)
    {
        m_buttonShape.setFillColor(sf::Color(100, 150, 100));
        m_text.setFillColor(sf::Color::White);
    }
    else
    {
        m_buttonShape.setFillColor(sf::Color(80, 80, 80));
        m_text.setFillColor(sf::Color(120, 120, 120));
    }
}