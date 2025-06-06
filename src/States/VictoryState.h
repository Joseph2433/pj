#pragma once
#include "Core/GameState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "../UI/Button.h"

class StateManager;
class ResourceManager;

class VictoryState : public GameState
{
public:
    VictoryState(StateManager *stateManager);
    ~VictoryState() override = default;

    void enter() override;
    void exit() override;
    void handleEvent(const sf::Event &event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

private:
    void setupUI();
    void executeAction(const std::string &action);

    sf::Font m_font;
    bool m_fontLoaded;

    sf::Text m_victoryText;
    sf::Text m_congratsText; // 额外祝贺文本
    std::vector<Button> m_buttons;
    sf::Vector2f m_mousePosition;

    sf::Sprite m_backgroundSprite;
};
