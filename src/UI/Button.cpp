#include "Button.h"

// 颜色定义（静态成员初始化）
const sf::Color Button::NORMAL_COLOR = sf::Color(100, 150, 100);
const sf::Color Button::HOVER_COLOR = sf::Color(120, 170, 120);
const sf::Color Button::DISABLED_COLOR = sf::Color(80, 80, 80);
const sf::Color Button::TEXT_COLOR = sf::Color::White;
const sf::Color Button::HOVER_TEXT_COLOR = sf::Color::Yellow;
const sf::Color Button::DISABLED_TEXT_COLOR = sf::Color(120, 120, 120);

Button::Button(const sf::Vector2f &position, const sf::Vector2f &size,
               const std::string &text, const sf::Font &font)
    : m_buttonShape(), m_text(text, font)
{
    // 设置按钮形状
    m_buttonShape.setPosition(position);
    m_buttonShape.setSize(size);
    // m_buttonShape.setFillColor(NORMAL_COLOR); // 外观由 updateAppearance 初始设置
    m_buttonShape.setOutlineThickness(2.f);
    m_buttonShape.setOutlineColor(sf::Color(50, 100, 50));

    // 设置文本样式
    m_text.setCharacterSize(24);
    // m_text.setFillColor(TEXT_COLOR); // 外观由 updateAppearance 初始设置

    // 文本居中对齐
    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setPosition(
        position.x + (size.x - textBounds.width) / 2.f,
        position.y + (size.y - textBounds.height) / 2.f - textBounds.top);

    updateAppearance(); // 在构造函数结束前调用一次，以设置初始外观
}

// 只保留一个 render (或 draw) 方法，并且它是 const 的
// 并且它内部不调用 updateAppearance()
void Button::render(sf::RenderWindow &window) const
{ // 或者叫 draw，但要与 .h 声明一致
    window.draw(m_buttonShape);
    window.draw(m_text);
}

// handleMouseMove, handleMouseClick, setCallback, setEnabled, isEnabled, updateAppearance 保持不变
void Button::handleMouseMove(const sf::Vector2f &mousePosition)
{
    if (!m_enabled)
        return;

    bool newHovered = m_buttonShape.getGlobalBounds().contains(mousePosition);
    if (newHovered != m_hovered)
    {
        m_hovered = newHovered;
        updateAppearance(); // 状态改变，更新外观
    }
}

void Button::handleMouseClick(const sf::Vector2f &mousePosition)
{
    if (m_enabled && m_hovered && m_callback)
    {
        m_callback();
    }
}

void Button::setCallback(std::function<void()> callback)
{
    m_callback = callback;
}

void Button::setEnabled(bool enabled)
{
    if (m_enabled != enabled)
    {
        m_enabled = enabled;
        if (!m_enabled)
        { // 如果按钮被禁用，也应该取消悬停状态
            m_hovered = false;
        }
        updateAppearance(); // 状态改变，更新外观
    }
}

bool Button::isEnabled() const
{
    return m_enabled;
}

void Button::updateAppearance()
{
    if (m_enabled)
    {
        if (m_hovered)
        {
            m_buttonShape.setFillColor(HOVER_COLOR);
            m_text.setFillColor(HOVER_TEXT_COLOR);
        }
        else
        {
            m_buttonShape.setFillColor(NORMAL_COLOR);
            m_text.setFillColor(TEXT_COLOR);
        }
    }
    else
    {
        m_buttonShape.setFillColor(DISABLED_COLOR);
        m_text.setFillColor(DISABLED_TEXT_COLOR);
    }
}
