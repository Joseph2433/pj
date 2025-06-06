#include "ProgressBar.h"
#include <algorithm> // For std::clamp (C++17) or manual clamp

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
    m_fillBar.setSize(sf::Vector2f(0, size.y)); // Initially empty
    m_fillBar.setFillColor(fillColor);

    // Default text properties (can be overridden)
    m_text.setFillColor(sf::Color::White);
    m_text.setCharacterSize(static_cast<unsigned int>(size.y * 0.6f)); // Default size based on bar height
    updateTextPosition();
}

void ProgressBar::setProgress(float progress)
{
// Clamp progress between 0.0 and 1.0
#if __cplusplus >= 201703L // C++17 and later
    m_currentProgress = std::clamp(progress, 0.0f, 1.0f);
#else // Fallback for older C++ versions
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
    updateTextPosition(); // Recalculate position if font changes bounds
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
    m_fillBar.setPosition(position); // Fill bar aligns with background
    updateTextPosition();
}

void ProgressBar::setSize(const sf::Vector2f &size)
{
    m_size = size;
    m_backgroundBar.setSize(size);
    // Update fill bar size based on current progress and new total size
    setProgress(m_currentProgress);
    updateTextPosition();
}

void ProgressBar::updateTextPosition()
{
    if (!m_text.getFont())
        return; // Don't try to position if no font

    sf::FloatRect textBounds = m_text.getLocalBounds();
    float textX = m_backgroundBar.getPosition().x + (m_size.x - textBounds.width) / 2.f;
    float textY = m_backgroundBar.getPosition().y + (m_size.y - textBounds.height) / 2.f - textBounds.top; // Adjust for text origin
    m_text.setPosition(textX, textY);
}

void ProgressBar::draw(sf::RenderWindow &window) const
{
    window.draw(m_backgroundBar);
    window.draw(m_fillBar);
    if (m_text.getFont() && !m_text.getString().isEmpty())
    { // Only draw text if font and string are set
        window.draw(m_text);
    }
}

void ProgressBar::setFillColor(const sf::Color &color)
{
    m_fillBar.setFillColor(color);
}
