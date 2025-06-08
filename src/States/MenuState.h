#pragma once
#include "Core/GameState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../UI/Button.h"

class StateManager;

class MenuState : public GameState
{
public:
    MenuState(StateManager *stateManager);

    void enter() override;
    void exit() override;
    void handleEvent(const sf::Event &event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

private:
    // 字体
    sf::Font m_font;
    bool m_useCustomFont = false;

    // 背景
    sf::Sprite m_BackgroundSpite;

    // UI元素
    sf::Text m_titleText;
    std::vector<Button> m_buttons;
    sf::Vector2f m_mousePosition;

    sf::Text setupText(const std::string &text, unsigned int size, sf::Color color, float yOffset) const;
    void setupButtons();
    void executeAction(const std::string &action);
};