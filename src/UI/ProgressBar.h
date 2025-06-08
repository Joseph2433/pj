#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ProgressBar
{
public:
    ProgressBar(const sf::Vector2f &position, const sf::Vector2f &size,
                sf::Color backgroundColor = sf::Color(100, 100, 100),
                sf::Color fillColor = sf::Color(50, 200, 50));

    void setProgress(float progress);
    float getProgress() const;

    void setText(const std::string &text);
    void setFont(const sf::Font &font);
    void setCharacterSize(unsigned int size);
    void setTextColor(const sf::Color &color);

    void setPosition(const sf::Vector2f &position);
    void setSize(const sf::Vector2f &size);

    void draw(sf::RenderWindow &window) const;
    void setFillColor(const sf::Color &color);

    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;

private:
    void updateTextPosition();

    sf::RectangleShape m_backgroundBar;
    sf::RectangleShape m_fillBar;
    sf::Text m_text;

    sf::Vector2f m_size;
    float m_currentProgress;
};
