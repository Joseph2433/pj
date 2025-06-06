#pragma once
#include "Core/GameState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "../UI/Button.h" // 假设 Button 类已存在且可用

class StateManager;
class ResourceManager; // 如果需要加载特定资源

class GameOverState : public GameState
{
public:
    GameOverState(StateManager *stateManager);
    ~GameOverState() override = default;

    void enter() override;
    void exit() override;
    void handleEvent(const sf::Event &event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow &window) override;

private:
    void setupUI();
    void executeAction(const std::string &action);

    sf::Font m_font; // 字体最好通过 ResourceManager 管理
    bool m_fontLoaded;

    sf::Text m_gameOverText;
    std::vector<Button> m_buttons;
    sf::Vector2f m_mousePosition;

    sf::Sprite m_backgroundSprite; // 背景
};
