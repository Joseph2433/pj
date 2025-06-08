#pragma once
#include "Core/GameState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "../UI/Button.h"

class StateManager;
class ResourceManager;

class PauseState : public GameState
{
public:
    PauseState(StateManager *stateManager);
    ~PauseState() override = default;

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

    sf::Text m_pauseText;
    std::vector<Button> m_buttons;
    sf::Vector2f m_mousePosition;
    // 半透明背景遮罩
    sf::RectangleShape m_backgroundOverlay;
};
