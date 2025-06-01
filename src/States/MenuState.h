#pragma once
#include "../Core/GameState.h"
#include <SFML/Graphics.hpp>

class MenuState : public GameState
{
public:
    MenuState();
    ~MenuState() override = default;

    void enter() override;
    void exit() override;
    void handleEvent(const sf::Event &event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

private:
    sf::Text m_titleText;
    sf::Text m_instructionText;
    sf::Text m_gridInfoText;
    bool m_showGridInfo;
    sf::Vector2i m_mouseGridPos;
};