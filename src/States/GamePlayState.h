#pragma once
#include "Core/GameState.h"
#include <SFML/Graphics.hpp>

class StateManager;

class GamePlayState : public GameState
{
public:
    GamePlayState(StateManager *stateManager);
    ~GamePlayState() override = default;

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
    sf::Text m_instructionText;
    sf::Text m_gameInfoText;

    // 游戏相关
    float m_gameTime;
    sf::Vector2i m_mousePos;
};