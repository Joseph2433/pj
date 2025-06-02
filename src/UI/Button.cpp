#include "Button.h"

// 颜色定义（静态成员初始化）
const sf::Color Button::NORMAL_COLOR = sf::Color(100, 150, 100);        // 深绿色
const sf::Color Button::HOVER_COLOR = sf::Color(120, 170, 120);         // 亮绿色
const sf::Color Button::DISABLED_COLOR = sf::Color(80, 80, 80);         // 灰色
const sf::Color Button::TEXT_COLOR = sf::Color::White;                  // 白色文本
const sf::Color Button::HOVER_TEXT_COLOR = sf::Color::Yellow;           // 黄色文本（悬停）
const sf::Color Button::DISABLED_TEXT_COLOR = sf::Color(120, 120, 120); // 浅灰色文本（禁用）

Button::Button(const sf::Vector2f &position, const sf::Vector2f &size,
               const std::string &text, const sf::Font &font)
    : m_buttonShape(), m_text(text, font)
{
    // 设置按钮形状
    m_buttonShape.setPosition(position);
    m_buttonShape.setSize(size);
    m_buttonShape.setFillColor(NORMAL_COLOR);
    m_buttonShape.setOutlineThickness(2.f);
    m_buttonShape.setOutlineColor(sf::Color(50, 100, 50)); // 更深的绿色边框

    // 设置文本样式
    m_text.setCharacterSize(24);
    m_text.setFillColor(TEXT_COLOR);

    // 文本居中对齐
    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setPosition(
        position.x + (size.x - textBounds.width) / 2.f,
        position.y + (size.y - textBounds.height) / 2.f - textBounds.top);
}

void Button::render(sf::RenderWindow &window)
{
    updateAppearance(); // 先更新外观
    window.draw(m_buttonShape);
    window.draw(m_text);
}

void Button::handleMouseMove(const sf::Vector2f &mousePosition)
{
    if (!m_enabled)
        return; // 禁用状态不处理悬停

    // 检测鼠标是否在按钮范围内
    bool newHovered = m_buttonShape.getGlobalBounds().contains(mousePosition);
    if (newHovered != m_hovered)
    {
        m_hovered = newHovered;
        updateAppearance(); // 状态变化时更新外观
    }
}

void Button::handleMouseClick(const sf::Vector2f &mousePosition)
{
    if (m_enabled && m_hovered && m_callback) // 仅在启用、悬停时触发回调
    {
        m_callback(); // 执行回调函数
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
        updateAppearance(); // 状态变化时更新外观
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