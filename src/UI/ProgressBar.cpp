#include "ProgressBar.h"
#include <algorithm>

ProgressBar::ProgressBar(const sf::Vector2f &position, const sf::Vector2f &size,
                         sf::Color backgroundColor, sf::Color fillColor)
    : m_size(size), m_currentProgress(0.0f)
{

    m_backgroundBar.setPosition(position);
    m_backgroundBar.setSize(size);
    m_backgroundBar.setFillColor(backgroundColor);
    m_backgroundBar.setOutlineThickness(1.f);
    m_backgroundBar.setOutlineColor(sf::Color(backgroundColor.r / 2, backgroundColor.g / 2, backgroundColor.b / 2));

    m_fillBar.setPosition(position);
    m_fillBar.setSize(sf::Vector2f(0, size.y));
    m_fillBar.setFillColor(fillColor);

    m_text.setFillColor(sf::Color::White);
    m_text.setCharacterSize(static_cast<unsigned int>(size.y * 0.6f));
    updateTextPosition();
}

void ProgressBar::setProgress(float progress)
{
// Clamp progress between 0.0 and 1.0
#if __cplusplus >= 201703L
    m_currentProgress = std::clamp(progress, 0.0f, 1.0f);
#else
    m_currentProgress = std::max(0.0f, std::min(progress, 1.0f));
#endif

    m_fillBar.setSize(sf::Vector2f(m_size.x * m_currentProgress, m_size.y));
}

float ProgressBar::getProgress() const
{
    return m_currentProgress;
}

void ProgressBar::setText(const std::string &text)
{
    m_text.setString(text);
    updateTextPosition();
}

void ProgressBar::setFont(const sf::Font &font)
{
    m_text.setFont(font);
    updateTextPosition();
}

void ProgressBar::setCharacterSize(unsigned int size)
{
    m_text.setCharacterSize(size);
    updateTextPosition();
}

void ProgressBar::setTextColor(const sf::Color &color)
{
    m_text.setFillColor(color);
}

void ProgressBar::setPosition(const sf::Vector2f &position)
{
    m_backgroundBar.setPosition(position);
    m_fillBar.setPosition(position);
    updateTextPosition();
}

void ProgressBar::setSize(const sf::Vector2f &size)
{
    m_size = size;
    m_backgroundBar.setSize(size);
    setProgress(m_currentProgress);
    updateTextPosition();
}

void ProgressBar::updateTextPosition()
{
    if (!m_text.getFont())
        return;

    sf::FloatRect textBounds = m_text.getLocalBounds();
    float textX = m_backgroundBar.getPosition().x + (m_size.x - textBounds.width) / 2.f;
    float textY = m_backgroundBar.getPosition().y + (m_size.y - textBounds.height) / 2.f - textBounds.top;
    m_text.setPosition(textX, textY);
}

void ProgressBar::draw(sf::RenderWindow &window) const
{
    window.draw(m_backgroundBar);
    window.draw(m_fillBar);
    if (m_text.getFont() && !m_text.getString().isEmpty())
    {
        window.draw(m_text);
    }
}

void ProgressBar::setFillColor(const sf::Color &color)
{
    m_fillBar.setFillColor(color);
}

sf::Vector2f ProgressBar::getPosition() const
{
    return m_backgroundBar.getPosition();
}
sf::Vector2f ProgressBar::getSize() const
{
    return m_backgroundBar.getSize();
}